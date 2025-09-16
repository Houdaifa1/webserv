# ifndef PARSER_HPP
# define PARSER_HPP

# include "Config.hpp"
# include "../token_hpp/Token.hpp"

class Parser
{

    private :

        std::vector<Token>  tokens;
        size_t              index;

    public :

        Parser(const std::vector<Token> &tokens);
        int Parseall(Config &config);
        // bool is_directive(Token &token);
        Directive parse_directive(const std::vector<Token> &tokens, size_t index);

};



# endif