#include "../../includes/webserv.hpp"


Connection::Connection(int client_fd, const Server &server,
               const std::string &client_ip, int client_port,
               const std::string &server_ip, int server_port) : 
            client_fd(client_fd), server(server),
            client_ip(client_ip), client_port(client_port),
            server_ip(server_ip), server_port(server_port),
            out_file(-1), out_chunk(), out_chunk_size(0), 
            out_offset(0), sending_file(false), out_file_size(0),
            out_file_pos(0) {last_activity = std::time(NULL);}