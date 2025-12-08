#include "../includes/webserv.hpp"


int main (int argc, char **argv)
{
    Config  config;
    int     exit_code;
    std::string config_file;
    
    
    if (argc == 1)
        config_file = "./config/default.conf";
    else if (argc == 2)
        config_file = argv[1];
    else
    {
        std::cerr << "Usage: ./webserv <config_file>\n";
        return 1;
    }
    exit_code = get_config(config_file, config);
    if (exit_code != 0) return (exit_code);
    exit_code = startserver(config);
    if (exit_code != 0) return (exit_code);
    return (exit_code);
}