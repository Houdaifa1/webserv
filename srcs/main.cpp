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
    if (exit_code != 0) return (exit_code);
    ServerCore servercore(config);
    
    return (exit_code);
}