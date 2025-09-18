# ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include "../token_hpp/Token.hpp"
# include "ParserError.hpp"

struct TrackPos
{
    int line;
    int col;
};

class Directive 
{
    public :

        std::string                 name;
        std::vector<std::string>    args;
        TrackPos                    position;

        Directive();

};

# endif