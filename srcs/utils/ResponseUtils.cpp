#include "../../includes/webserv.hpp"

void send_simple_response(int fd, const std::string &status_line)
{
    std::string response =
        "HTTP/1.1 " + status_line + "\r\n"
        "Content-Length: 0\r\n"
        "\r\n";
    send(fd, response.c_str(), response.size(), 0);
}


void send_created_response(int fd, const std::string &location)
{
    std::ostringstream ss;
    ss << "HTTP/1.1 201 Created\r\n"
       << "Content-Length: 0\r\n"
       << "Location: " << location << "\r\n"
       << "\r\n";
    std::string response = ss.str();
    send(fd, response.c_str(), response.size(), 0);
}