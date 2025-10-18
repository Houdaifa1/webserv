# ifndef ERRORHANDLER_HPP
# define ERRORHANDLER_HPP


# include "../webserv.hpp"


class ErrorHandler
{
    private :

        Location    &location;
        Connection  &connection;
        std::string body;

    public :

        ErrorHandler(Location &location, Connection &connection);
        void    generate_error_response(int error_code);

};




# endif 