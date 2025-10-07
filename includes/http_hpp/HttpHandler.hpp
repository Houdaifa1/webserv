# ifndef HTTPHANDLER_HPP
# define HTTPHANDLER_HPP

# include "../webserv.hpp"

class HttpHandler
{
    private :

        Connection &connection;

    public :

        HttpHandler(Connection &connection);

        bool handle_get();
        bool handle_post();
        bool handle_delete();
};



# endif 