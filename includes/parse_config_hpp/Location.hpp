# ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Directive.hpp"

class Location
{
    public:

        std::string                         path;
        std::string                         root;
        std::string                         autoindex;
        std::map<std::string, std::string>  errors;
        std::vector<Directive>              directives;
        TrackPos                            position;    
};

# endif