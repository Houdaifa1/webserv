# ifndef HTTPHANDLER_HPP
# define HTTPHANDLER_HPP

# include "../webserv.hpp"

class HttpHandler
{
    private :

        Connection &connection;

    public :

        HttpHandler(Connection &connection);

        void handle_get();
        void handle_post();
        void handle_delete();
};



# endif 