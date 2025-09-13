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
        int                 done;
        char                c;

        int     tokenize_str();
        int     get_config_in_str();
        void    skip_void();
        void    is_comment();
        void    is_symbol();
        void    is_digit();
        int     is_only_digits(const std::string &value);
        int     is_size_digit(const std::string &value, const std::string  &allowed);
        void    is_path();
        int     not_end_value();
        void    is_identifier();
        int     valid_identifier_char();

    public :

        Tokenizer(const std::string &file_name);
        int Tokenizeall();
        void display_config();
  

};




# endif 