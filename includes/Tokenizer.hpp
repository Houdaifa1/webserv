# ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include "Token.hpp"
# include "TokenizerError.hpp"

class Tokenizer
{
    private :
        
        std::string         file_name;
        std::string         config;
        std::vector<Token>  tokens;
        int                 index;
        int                 col;
        int                 line;
        char                c;

        void     tokenize_str();
        void     get_config_in_str();
        bool    consume_whitespaces();
        bool    consume_comment();
        bool    consume_symbol();
        bool    consume_digit();
        int     is_only_digits(const std::string &value);
        int     is_size_digit(const std::string &value, const std::string  &allowed);
        bool    consume_path();
        int     not_end_value();
        bool    consume_identifier();
        bool    consume_quotes();
        int     valid_identifier_char();
        bool    is_location_modifier(std::string &value);

    public :

        Tokenizer(const std::string &file_name);
        int Tokenizeall();
        void display_config();
  

};




# endif 