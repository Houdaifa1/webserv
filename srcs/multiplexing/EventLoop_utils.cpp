#include "../../includes/webserv.hpp"


void rearm_epollout(int epoll_fd, int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) < 0)
    {
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    }
}
