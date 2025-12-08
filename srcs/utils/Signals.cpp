#include "../../includes/webserv.hpp"
#include "../../includes/utils_hpp/Signals.hpp"

bool    g_stop = false;

void    SignalHandler(int sig)
{
    g_stop = true;
    if (sig == SIGINT)
        std::cout << "\nSIGINT CALLED" << std::endl;
    else if(sig == SIGQUIT)
        std::cout << "\nSIGQUIT CALLED" << std::endl;
}
