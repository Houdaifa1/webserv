# ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "../parse_config_hpp/Parser.hpp"
# include "./CoreError.hpp"

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
        std::string     request;
        std::string     response;
        ConnState       state;
        size_t          bytesSent;
};


# endif 