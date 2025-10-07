#include "../../includes/webserv.hpp"

Connection::Connection(int client_fd, const Server &server,
               const std::string &client_ip, int client_port,
               const std::string &server_ip, int server_port) : 
            client_fd(client_fd), server(server),
            client_ip(client_ip), client_port(client_port),
            server_ip(server_ip), server_port(server_port) {}