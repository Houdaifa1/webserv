#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../webserv.hpp"
#include "Environment.hpp"

class CgiHandler {
    private:
        const std::string name;
    public:
        Environment environment;
        CgiHandler(Connection &conn, HttpRequest &req);
        std::vector<std::string> GetEnv();
        
};

#endif