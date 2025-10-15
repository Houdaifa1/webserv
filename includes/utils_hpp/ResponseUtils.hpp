#ifndef RESPONSE_UTILS_HPP
#define RESPONSE_UTILS_HPP

#include "../Libraries.hpp"

void send_simple_response(int fd, const std::string &status_line);
void send_created_response(int fd, const std::string &location);

#endif