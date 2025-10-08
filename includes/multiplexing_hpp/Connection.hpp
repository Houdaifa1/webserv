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
        
        HttpRequest     request;
        std::string     buffer;
        // HttpResponse response;
        ConnState       state;

        Connection(int client_fd, const Server &server,
               const std::string &client_ip, int client_port,
               const std::string &server_ip, int server_port);
};


# endif 