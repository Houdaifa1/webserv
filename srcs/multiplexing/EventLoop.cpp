#include "../../includes/multiplexing_hpp/EventLoop.hpp"
#include "../../includes/webserv.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

EventLoop::EventLoop(ServerCore &srv) : server(srv)
{
    epoll_fd = epoll_create1(0);
    std::cout << "epoll_fd = " << epoll_fd << std::endl;
    if (epoll_fd < 0)
    {
        perror("epoll_create1");
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
            perror("epoll_ctl listen_fd");
            std::exit(1);
        }
        listening_fds.insert(listen_fd);
        std::cout << "Listening on FD: " << listen_fd << std::endl;
    }
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

    std::cout << "Accepted client FD: " << client_fd << std::endl;
}

void EventLoop::handle_client(int client_fd)
{
    char buffer[4096];
    int bytes;
    std::map<int, Connection>::iterator it = connections.find(client_fd);
    if (it == connections.end())
        return; 
    Connection &connection = it->second;
    
    while (true)
    {
        bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes > 0)
            connection.buffer.append(buffer, bytes);
        else if (bytes == 0)
        {
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
            close(client_fd);
            connections.erase(it);
            std::cout << "client closed connection: " << client_fd << std::endl;
            return;
        }
        else
        {
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
            close(client_fd);
            connections.erase(it);
            std::cout << "recv() failed on client " << client_fd << ", closing connection" << std::endl;
            return; 
        }
        if (bytes < (int)sizeof(buffer))
            break;

    }
    RequestResult result = parse_http_request(connection.buffer, connection.request);
   
    if (result == SUCCESS)
    {
        HttpHandler handlereq(connection);
        CgiHandler cgi;
        cgi.SetEnv(connection, connection.request);

        
        std::cout << "\n******************* ENV *********************\n";

        cgi.PrintEnv();
        std::cout << "########### END ############" << std::endl;

        // std::cout << "\n***************** Request *******************\n";
        // std::cout << "Method: " << connection.request.get_httpmethod() << "\n";
        // std::cout << "Version: " << connection.request.get_httpversion() << "\n";
        // std::cout << "Body: " << connection.request.get_body() << "\n";
        // std::cout << "Query: " << connection.request.get_query() << "\n";
        // std::cout << "Request Path: " << connection.request.get_requestpath() << "\n";
        // std::cout << "Correct Path: " << connection.request.get_correct_path() << "\n";
        // std::cout << "Host Domain: " << connection.request.get_host_domain() << "\n";
        // std::cout << "Host Port: " << connection.request.get_host_port() << "\n";
        // std::cout << "Server IP: " << connection.server_ip << "\n";
        // std::cout << "Server Port: " << connection.server_port << "\n";
        // std::cout << "Client IP: " << connection.client_ip << "\n";
        // std::cout << "CLient Port: " << connection.client_port << "\n";
        // std::cout << "CLient fd: " << connection.client_fd << "\n";
        // std::cout << "######### Headers ###########" << std::endl;
        // for (const auto &pair : connection.request.get_headers()) {
        //     std::cout << pair.first << " = " << pair.second << std::endl;
        // }
        // std::cout << "########### END ############" << std::endl;
        
        std::cout << "Parsed request from client " << client_fd << std::endl;
        std::cout << "Method: " << connection.request.get_httpmethod() << std::endl;
        std::cout << "Path: " << connection.request.get_requestpath() << std::endl;
        std::cout << "Version: " << connection.request.get_httpversion() << std::endl;
        
        connection.buffer.clear();
        close(client_fd);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
        connections.erase(it);
        // todo after method: add timeouts , and keep-alive (3lahsab) 
    }
    else if (result == INCOMPLETE)
    {
        std::cout << "Incomplete request from client " << client_fd << std::endl;
        return;
    }
    else 
    {
        std::cout << "Bad request from client " << client_fd << std::endl;
        std::string response =
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Length: 0\r\n"
            "\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
        close(client_fd);
        connections.erase(it);
    }
    
}

void EventLoop::run()
{
    int fd;
    int i;
    const int MAX_EVENTS = 64;
    std::vector<struct epoll_event> events(MAX_EVENTS);

    while (true)
    {
        signal(SIGINT, SignalHandler);
        signal(SIGQUIT, SignalHandler);
        int n = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
        if (n < 0)
        {
            perror("epoll_wait");
            break;
        }
        for (i = 0; i < n; i++)
        {
            fd = events[i].data.fd;
            if (listening_fds.count(fd))
            {
                accept_client(fd);
            }
            else
            {
                handle_client(fd);
            }
        }
    }
}
