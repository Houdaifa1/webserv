#include "../../includes/webserv.hpp"



int startserver(const Config &config)
{
    ServerCore servercore(config);
    try 
    {
        servercore.init_sockets();
    }
    catch (CoreError &error)
    {
        std::cout << error.mesg;
        return 1;
    }
    return 0;
}