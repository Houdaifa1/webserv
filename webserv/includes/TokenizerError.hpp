# ifndef TOKENIZERERROR_HPP
# define TOKENIZERERROR_HPP

# include "Libraries.hpp"

class TokenizerError
{
    public :

        std::string mesg;

        TokenizerError(int col, int line, char badchar);
};






# endif 