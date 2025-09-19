# ifndef PARSERERROR_HPP
# define PARSERERROR_HPP

# include "../Libraries.hpp"

enum ParserErrorType
{
    UnknownDirective,
    DirectiveNotTerminated,
    UnexpectedEOF,
    UnexpectedEOFend,
    ExpectBlockStarter,
    UnexpectedSymbol,
    ExpectedPath,
    NestedBlocks,
    TopBolockLow,
};

class Parsererror
{
    public :
    
        std::string mesg;

        Parsererror(ParserErrorType type, std::string value, std::string &path,
                    int line);


};


# endif