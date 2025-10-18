#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../webserv.hpp"

class CgiHandler {
    private:
        std::vector<std::string> env;
        void SetHeader (std::vector<std::string> &env, HttpRequest &req);
    public:
        CgiHandler();
        void SetEnv(Connection &conn, HttpRequest &req);    
};

#endif