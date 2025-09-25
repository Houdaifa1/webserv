# ifndef WEBSERv_HPP
# define WEBSERv_HPP

# include "token_hpp/Tokenizer.hpp"
# include "parse_config_hpp/Parser.hpp" 

int     get_config(char *file_name, Config  &config);
void    parse_listen(Directive &directive, std::string &path);

# endif