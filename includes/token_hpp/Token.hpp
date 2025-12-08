# ifndef TOKEN_HPP
# define TOKEN_HPP

# include "../Libraries.hpp"

enum    TokenType
{
    UNDEFINED,
    IDENTIFIER,
    STRING,
    SYMBOL,
    NUMBER,
    SIZE,
    PATH,
    COMMENT,
    LOCATION_MODIFIER,
};


class   Token
{
    public :

        TokenType   type;
        std::string value;
        int         line;
        int         col;
        Token();
        void    set_new_token(const std::string &value, int col, int line, 
        TokenType type);
        void    display_token();
};


# endif