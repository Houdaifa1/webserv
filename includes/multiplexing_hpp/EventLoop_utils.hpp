#ifndef EVENTLOOP_UTILS_HPP
#define EVENTLOOP_UTILS_HPP

#include "../webserv.hpp"

void rearm_epollout(int epoll_fd, int fd);

#endif