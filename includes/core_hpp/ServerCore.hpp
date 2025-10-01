# ifndef SERVERCORE_HPP
# define SERVERCORE_HPP

# include "./Connection.hpp"

class ServerCore
{
    public :
        const Config                                &config;
        std::map<std::pair<std::string, int>, int>  listen_sockets;
        std::map<int, Connection>                   connections;


        ServerCore(const Config &config);
        void init_sockets();
        const Server &findserver(std::string ip, int port);
        //int acceptnewconnection(int listen_fd);
};

# endif