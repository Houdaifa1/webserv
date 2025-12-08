#ifndef RESPONSE_UTILS_HPP
#define RESPONSE_UTILS_HPP

#include "../Libraries.hpp"

void send_created_html(int client_fd, const std::string &path);
void    send_delete_confirmation(int client_fd, const std::string &path);

#endif