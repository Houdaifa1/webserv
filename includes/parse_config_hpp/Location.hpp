# ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Directive.hpp"

class Location
{
    private:

        std::string             modifier;
        std::string             path;
        std::vector<Directive>  directives;
        TrackPos                position;
    
    public:

        Location();
    
};

# endif