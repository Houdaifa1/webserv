# ifndef SERVERCORE_HPP
# define SERVERCORE_HPP

# include "../parse_config_hpp/Parser.hpp"
# include "./CoreError.hpp"

class ServerCore
{
    public :
        const Config                                &config;
        std::map<std::pair<std::string, int>, int>  listen_sockets;


        ServerCore(const Config &config);
        int init_sockets();

};

# endif