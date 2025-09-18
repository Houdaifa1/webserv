# ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Directive.hpp"

class Location
{
    public:

        std::string             path;
        std::vector<Directive>  directives;
        TrackPos                position;
    
   

    
};

# endif