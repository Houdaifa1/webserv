#include "../../includes/webserv.hpp"

int get_config(char *file_name, Config  &config)
{
    int exit_code = 0;

    Tokenizer tokens(file_name);
    exit_code = tokens.Tokenizeall();
    if (exit_code == 1)
        return (1);
    Parser parse_config(tokens.get_tokens(), file_name);
    exit_code = parse_config.Parseall(config);
    if (exit_code == 1)
        return (1);
    return 0;
}
