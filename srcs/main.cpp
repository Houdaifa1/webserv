#include "../includes/webserv.hpp"


int main (int argc, char **argv)
{
    Config  config;
    int     exit_code;

    if (argc < 2)
    {
        std::cerr << "Usage: ./webserv <config_file>\n";
        return 1;
    }
    exit_code = get_config(argv[1], config);
    // config.display_config();
    return (exit_code);
}