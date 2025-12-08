#ifndef LOGS_HPP
#define LOGS_HPP


#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define GRAY    "\033[90m"
#define GOLD "\033[38;2;255;215;0m"


void logServerStart();
void logListening(const std::string& ip, int port);
void logClientDisconnected(int fd);
void logClientConnected(const std::string& ip, int port, int fd);
void logReady();
void logHttpRequest(const std::string& method, const std::string& path, const std::string& client_ip, int fd);


#endif