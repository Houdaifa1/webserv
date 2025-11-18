#include "../../includes/webserv.hpp"
#include "../../includes/utils_hpp/Signals.hpp"

bool    g_stop = false;

void    SignalHandler(int sig)
{
    g_stop = true;
    if (sig == SIGINT)
        std::cout << "\n**** SIGINT CALLED ****" << std::endl;
    else if(sig == SIGQUIT)
        std::cout << "\n**** SIGQUIT CALLED ****" << std::endl;
}
