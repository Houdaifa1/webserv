# ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include "../token_hpp/Token.hpp"

struct TrackPos
{
    int line;
    int column;
};

class Directive 
{
    private :

        std::string                 name;
        std::vector<std::string>    args;
        TrackPos                    position;

    public :

        Directive();

};

# endif