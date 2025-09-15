#include "webserv.hpp"


int get_config(char *file_name)
{
    Tokenizer test(file_name);
    int exit_code = test.Tokenizeall();
    if (exit_code == 1)
        return (1);
    test.display_config();
}
