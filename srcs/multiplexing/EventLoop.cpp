#include "../../includes/multiplexing_hpp/EventLoop.hpp"
#include "../../includes/webserv.hpp"
#include "../../includes/utils_hpp/Signals.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
# include "../../includes/cgi/cgihandler.hpp"

void EventLoop::shutdown()
{
    std::map<int, Connection>::iterator it = connections.begin();
    for (; it != connections.end(); ++it)
    {
        int fd = it->first;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
    }
    connections.clear();
    std::set<int>::iterator sit = listening_fds.begin();
    while (sit != listening_fds.end())
    {
        int fd = *sit;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        ++sit;
    }
    listening_fds.clear();
    close(epoll_fd);
}


void    EventLoop::check_timeouts()
{
    const int   READ_TIMEOUT = 11;
    std::vector<int> expired_fds;
    time_t now = std::time(NULL);

    std::map<int, Connection>::iterator it = connections.begin();
    for (; it != connections.end(); ++it)
    {
        int fd = it->first;
        if (listening_fds.count(fd))
            continue;
        
        Connection &conn = it->second;
        if (now - conn.last_activity >= READ_TIMEOUT)
            expired_fds.push_back(fd);
    }
    for (size_t i = 0; i < expired_fds.size(); ++i)
    {
        int fd = expired_fds[i];
        std::map<int, Connection>::iterator it =  connections.find(fd);
        if (it != connections.end())
        {
            Connection &conn = it->second;
            ErrorHandler    error_mesg(conn.location, conn);
            error_mesg.generate_error_response(408);
        }
        cleanup_connection(fd);
    }
}

void    EventLoop::update_activity(int fd)
{
    std::map<int, Connection>::iterator it = connections.find(fd);
    if (it != connections.end())
        it->second.last_activity = std::time(NULL);
}

EventLoop::EventLoop(ServerCore &srv) : server(srv)
{
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
    {
        std::cerr << "epoll_create1" << std::endl;
        std::exit(1);
    }
    struct epoll_event ev;
    std::map<std::pair<std::string, int>, int>::iterator it;
    for (it = server.listen_sockets.begin(); it != server.listen_sockets.end(); ++it)
    {
        int listen_fd = it->second;
        set_nonblocking(listen_fd);

        ev.events = EPOLLIN;
        ev.data.fd = listen_fd;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
        {
            std::cerr << "epoll_ctl listen_fd" << std::endl;
            std::exit(1);
        }
        listening_fds.insert(listen_fd);
    }
    logReady();
}

EventLoop::~EventLoop()
{
    close(epoll_fd);
}

void EventLoop::set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void EventLoop::cleanup_connection(int fd)
{
    std::map<int, Connection>::iterator it = connections.find(fd);
    if (it == connections.end())
        return;

    Connection& conn = it->second;
    if (conn.out_file >= 0)
    {
        close(conn.out_file);
        conn.out_file = -1;
    }
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
    connections.erase(it);
    logClientDisconnected(fd);
}


void EventLoop::create_connection(int client_fd)
{
    connections.erase(client_fd);
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    if (getpeername(client_fd, (struct sockaddr *)&client_addr, &client_len) < 0)
    {
        close(client_fd);
        return;
    }
    std::string client_ip = inet_ntoa(client_addr.sin_addr);
    int client_port = ntohs(client_addr.sin_port);

    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    if (getsockname(client_fd, (struct sockaddr *)&server_addr, &server_len) < 0)
    {
        close(client_fd);
        return;
    }
    std::string server_ip = inet_ntoa(server_addr.sin_addr);
    int server_port = ntohs(server_addr.sin_port);

    const Server &serv = server.findserver(server_ip, server_port);
    Connection conn(client_fd, serv, client_ip, client_port, server_ip, server_port);
    connections.insert(std::make_pair(client_fd, conn));

}

void EventLoop::accept_client(int listen_fd)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int client_fd = accept(listen_fd, (struct sockaddr *)&addr, &addrlen);
    if (client_fd < 0)
        return;

    set_nonblocking(client_fd);
    create_connection(client_fd);

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = client_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);

    std::map<int, Connection>::iterator it = connections.find(client_fd);
    if (it == connections.end())
        return; 
    Connection &connection = it->second;
    logClientConnected(connection.client_ip, connection.client_port, connection.client_fd);
    connection.last_activity = std::time(NULL);
    
}

void EventLoop::handle_client(int client_fd)
{
    char buffer[4096];
    int  bytes;
    std::map<int, Connection>::iterator it = connections.find(client_fd);
    if (it == connections.end())
        return; 
    Connection &connection = it->second;
    
    while (true)
    {
        bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes > 0)
        {
            connection.buffer.append(buffer, bytes);
            update_activity(client_fd);
        }
        else if (bytes == 0)
        {
            cleanup_connection(client_fd);
            return;
        }
        else
            break;
        if (bytes < (int)sizeof(buffer))
            break;
    }
    RequestResult result = parse_http_request(connection.buffer, connection.request);
    if (result == SUCCESS)
    {
        HttpHandler handlereq(connection);
        connection.buffer.clear();
        if (connection.sending_file)
        {
            struct epoll_event ev;
            ev.events = EPOLLIN | EPOLLOUT;
            ev.data.fd = client_fd;
            if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &ev) < 0)
            {
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0)
                {
                    cleanup_connection(client_fd);
                    return;
                }
            }
            return ;
        }
        cleanup_connection(client_fd);
        return;
    }
    else if (result == INCOMPLETE)
        return;
    else 
    {
        ErrorHandler error_mesg(connection.location, connection);
        error_mesg.generate_error_response(400);
        cleanup_connection(client_fd);
    }
    
}

void EventLoop::handle_write(int client_fd)
{
    std::map<int, Connection>::iterator it = connections.find(client_fd);
    if (it == connections.end()) return;

    Connection &conn = it->second;

    if (!conn.sending_file || conn.out_file < 0)
        return;

    const size_t CHUNK_SIZE = 8192;
    char buf[CHUNK_SIZE];
    if (conn.out_chunk_size > conn.out_offset)
    {
        size_t remaining = conn.out_chunk_size - conn.out_offset;
        const char *p = conn.out_chunk.c_str() + conn.out_offset;
        ssize_t s = send(client_fd, p, remaining, 0);

        if (s > 0)
        {
            conn.out_offset += (size_t)s;
            conn.out_file_pos += (size_t)s;

            if (conn.out_offset < conn.out_chunk_size)
            {
                rearm_epollout(epoll_fd, client_fd);
                return;
            }
            conn.out_chunk.clear();
            conn.out_chunk_size = 0;
            conn.out_offset = 0;
        }
        else
        {
            rearm_epollout(epoll_fd, client_fd);
            return;
        }
    }

    ssize_t nread = read(conn.out_file, buf, CHUNK_SIZE);
    if (nread <= 0)
    {
        cleanup_connection(client_fd);
        return;
    }
    ssize_t sent = send(client_fd, buf, nread, 0);
    if (sent > 0)
    {
        conn.out_file_pos += (ssize_t)sent;

        if (sent < nread)
        {
            conn.out_chunk.assign(buf + sent, nread - sent);
            conn.out_chunk_size = nread - sent;
            conn.out_offset = 0;
            rearm_epollout(epoll_fd, client_fd);
            return;
        }
        if (conn.out_file_pos >= conn.out_file_size)
        {
            cleanup_connection(client_fd);
            return;
        }
        rearm_epollout(epoll_fd, client_fd);
        return;
    }
    else
    {
        conn.out_chunk.assign(buf, nread);
        conn.out_chunk_size = nread;
        conn.out_offset = 0;
        rearm_epollout(epoll_fd, client_fd);
        return;
    }
}

void EventLoop::run()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, SignalHandler);
    signal(SIGQUIT, SignalHandler);
    int fd;
    int i;
    const int MAX_EVENTS = 64;
    const int EPOLL_TIMEOUT_MS = 1000;
    std::vector<struct epoll_event> events(MAX_EVENTS);
    while (!g_stop)
    {
        int n = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, EPOLL_TIMEOUT_MS);
        if (n < 0)
        {
            std::cerr << "Server shutdown" << std::endl;
            break;
        }
        for (i = 0; i < n; i++)
        {
            fd = events[i].data.fd;
            uint32_t ev = events[i].events;
            if (listening_fds.count(fd))
            {
                accept_client(fd);
                continue;
            }
            if (ev & (EPOLLERR | EPOLLHUP))
            {
                cleanup_connection(fd);
                continue;
            }
            if (ev & EPOLLIN)
            {
                handle_client(fd);
            }
            if (ev & EPOLLOUT)
            {
                handle_write(fd);
            }
        }
        check_timeouts();
    }
    shutdown();

}
