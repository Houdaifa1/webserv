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
        exit(1);
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
            exit(1);
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

void EventLoop::accept_client(int listen_fd)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int client_fd = accept(listen_fd, (struct sockaddr *)&addr, &addrlen);
    if (client_fd < 0)
        return;

    set_nonblocking(client_fd);

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = client_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);

    std::cout << "Accepted client FD: " << client_fd << std::endl;
}

void EventLoop::handle_client(int client_fd)
{
    char buffer[4096];
    int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes <= 0)
    {
        close(client_fd);
        std::cout << "Closed client FD: " << client_fd << std::endl;
        return;
    }
    buffer[bytes] = '\0';
    std::cout << "Received from client " << client_fd << ": " << buffer << std::endl;
    std::string request_str(buffer);

    HttpRequest req;
    RequestResult result = parse_http_request(request_str, req);

    if (result == SUCCESS)
    {
        std::cout << "Parsed request from client " << client_fd << std::endl;
        std::cout << "Method: " << req.get_httpmethod() << std::endl;
        std::cout << "Path: " << req.get_requestpath() << std::endl;
        std::cout << "Version: " << req.get_httpversion() << std::endl;

        // send(client_fd, response.c_str(), response.size(), 0);
    }
    else if (result == INCOMPLETE)
    {
        std::cout << "Incomplete request from client " << client_fd << std::endl;
    }
    else
    {
        std::cout << "Bad request from client " << client_fd << std::endl;
        std::string response =
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Length: 0\r\n"
            "\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
        close(client_fd);
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
