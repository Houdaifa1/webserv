# ifndef SERVERCORE_HPP
# define SERVERCORE_HPP

# include "../parse_config_hpp/Parser.hpp"

class ServerCore
{
    public :
        const Config &config;

        ServerCore(const Config &config);
        void init_sockets();

};

# endif