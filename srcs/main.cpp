#include "../includes/webserv.hpp"


int main (int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./webserv <config_file>\n";
        return 1;
    }
    Config  config;
    return (get_config(argv[1], config));

}