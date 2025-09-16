# ifndef SERVER_HPP
# define SERVER_HPP

# include "Location.hpp"

class   Server
{
    private :

        std::vector<Directive>  directives;
        std::vector<Location>   locations;
        TrackPos                position;

    public :

        Server();
};



# endif