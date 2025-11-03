#include "../../includes/webserv.hpp"

void    SignalHandler(int sig){
    if (sig == SIGINT){
        std::cout << "\n**** SIGINT CALLED ****" << std::endl;
        std::exit(1);
    }
    else if(sig == SIGQUIT){
        std::cout << "\n**** SIGQUIT CALLED ****" << std::endl;
        std::exit(1);
    }
}
