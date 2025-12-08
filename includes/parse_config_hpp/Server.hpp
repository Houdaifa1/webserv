# ifndef SERVER_HPP
# define SERVER_HPP

# include "Location.hpp"

class   Server
{
    public :

        std::vector<Directive>  directives;
        std::string             global_root;
        std::vector<Location>   locations;
        TrackPos                position;

};



# endif