#include "../../includes/webserv.hpp"

void logServerStart()
{
    std::cout << BOLD << CYAN << "🌀 Starting Webserv..." << RESET << std::endl;
}

void logListening(const std::string& ip, int port)
{
    std::cout <<  "✅ Listening on " << BOLD  << RED << "⍟⁞⌦" << YELLOW << "http://"
              << BOLD << ip << ":" << port <<  BOLD << RED << "⌫⁞⍟ " << RESET << std::endl;
}

void logReady()
{
    std::cout << MAGENTA << "🚀 Server is running and waiting for connections..." << RESET << std::endl;
}

void logClientConnected(const std::string& ip, int port, int fd)
{
    std::cout << GREEN << "🔗 Client connected " << RESET
              << "(" << CYAN << ip <<  RESET
              << ":" << CYAN << port << RESET
              << ", fd="<< YELLOW << fd << RESET << ")" << std::endl;
}

void logClientDisconnected(int fd)
{
    std::cout << GOLD << "❌ Client disconnected " << RESET
              << "fd= " << fd << std::endl;
}


void logHttpRequest(const std::string& method, const std::string& path, const std::string& client_ip, int fd)
{
    std::string color;

    if (method == "GET")
        color = BLUE;
    else if (method == "POST") 
        color = GREEN;
    else if (method == "DELETE") 
        color = RED;
    else
        color = MAGENTA;

    std::cout << color << "📥 " << method << RESET
              << " request from " << CYAN << client_ip  << RESET
              << " FD: " << CYAN << fd << RESET
              << " → " << BOLD << MAGENTA << path << RESET << std::endl;
}
