# ifndef PARSER_HPP
# define PARSER_HPP

# include "Config.hpp"

class Parser
{

    private :

        std::vector<Token>  tokens;
        size_t              index;
        std::string         path;

    public :

        Parser(const std::vector<Token> &tokens, std::string path);
        int Parseall(Config &config);
        bool is_serever_block(Token &token);
        bool is_directive(Token &token);
        bool is_known_directive(std::string &value);
        Directive parse_directive(const std::vector<Token> &tokens, size_t &index);

};



# endif