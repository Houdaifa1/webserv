# include "../includes/Tokenizer.hpp"


int main (int argc, char **argv)
{
    if (argc != 2)
    {
        return 1;
    }
    
    Tokenizer test(argv[1]);
    int code = test.Tokenizeall();
     test.display_config();
    return code;
}