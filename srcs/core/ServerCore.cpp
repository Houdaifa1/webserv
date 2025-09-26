# include "../../includes/webserv.hpp"


ServerCore::ServerCore(const Config &config) : config(config) {}




void ServerCore::init_sockets()
{
    std::map<std::pair<std::string, int>, int> listen_sockets;
    for (int i = 0; i < config.pairs.size(); i++)
    {
        if (listen_sockets.find(config.pairs[i]) == listen_sockets.end())
        {
            //not found means new key create a socket and add the fd to the map

        }
    }
}