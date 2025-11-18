#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

#include "../../includes/core_hpp/ServerCore.hpp"
# include "./Connection.hpp"
#include <sys/epoll.h>
#include <vector>
#include <set>

class EventLoop
{
    private:
        int epoll_fd;
        ServerCore &server;
        std::set<int> listening_fds;
        std::map<int, std::string>  client_buffers;
        std::map<int, Connection>   connections;
    public:
        EventLoop(ServerCore &srv);
        ~EventLoop();
        void run();

    private:
        void    set_nonblocking(int fd);
        void    accept_client(int listen_fd);
        void    handle_client(int client_fd);
        void    create_connection(int client_fd);
        void    handle_write(int client_fd);
        void    cleanup_connection(int fd);
        void    check_timeouts();
        void    update_activity(int fd);
        void    shutdown();
};

#endif