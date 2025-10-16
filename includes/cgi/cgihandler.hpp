#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../webserv.hpp"

class CgiHandler {
    private:
        std::vector<std::string> env;
    public:
        CgiHandler();
        void SetHeader (std::vector<std::string> &env, HttpRequest &req);
        void SetEnv(HttpRequest &req);
        
};

#endif