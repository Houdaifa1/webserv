# ifndef TOKENIZERERROR_HPP
# define TOKENIZERERROR_HPP

# include "Libraries.hpp"

enum    ErrorType
{
    FileNotFound,
    EmptyFile,
    InvalidIdentifierChar,
    UnclosedQuote,
    InvalidChar,
};

class TokenizerError
{
    public :

        std::string mesg;

        TokenizerError(ErrorType type, int col, int line, char badchar = '\0');
        std::string intToString(int n);
};






# endif 