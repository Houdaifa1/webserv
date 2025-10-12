# ifndef HTTPHANDLER_HPP
# define HTTPHANDLER_HPP

# include "../webserv.hpp"

class HttpHandler
{
    private :

        Connection &connection;

    public :

        HttpHandler(Connection &connection);

        void correct_path();
        void handle_get();
        bool decode_path(std::string &path);
        void handle_post();
        void handle_delete();
        void check_final_path();
};



# endif 