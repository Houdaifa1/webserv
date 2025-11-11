#include "../../includes/webserv.hpp"
#include "../../includes/multiplexing_hpp/EventLoop.hpp"


int startserver(const Config &config)
{
    ServerCore servercore(config);
    logServerStart();
    try 
    {
        servercore.init_sockets();
        EventLoop loop(servercore);
        loop.run();
    }
    catch (CoreError &error)
    {
        std::cout << error.mesg;
        return 1;
    }
    return 0;
}