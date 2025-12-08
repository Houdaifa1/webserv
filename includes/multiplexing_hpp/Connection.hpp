# ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include "../../includes/http_hpp/HttpRequest.hpp"

enum ConnState
{
    READING,
    WRITING,
    CLOSED,  
};

class Connection
{
    public :

        int             client_fd;
        const Server    &server;
        std::string     client_ip;
        int             client_port;
        std::string     server_ip;
        int             server_port;
        
        Location        location;
        std::string     request_full_path;
        HttpRequest     request;
        std::string     buffer;
        // HttpResponse response;
        ConnState       state;

        int     out_file;
        std::string out_chunk;
        size_t out_chunk_size;
        size_t  out_offset;
        bool    sending_file;
        off_t   out_file_size;
        off_t   out_file_pos;
        time_t  last_activity;

        Connection(int client_fd, const Server &server,
               const std::string &client_ip, int client_port,
               const std::string &server_ip, int server_port);
               
};


# endif 