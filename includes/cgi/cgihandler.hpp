#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../webserv.hpp"

class CgiHandler {
    private:
        std::vector<std::string> env;
        void SetHeaders(std::vector<std::string> &env, HttpRequest &req);
        void SetHttpHeaders(std::vector<std::string> &env, HttpRequest &req);
    public:
        CgiHandler();
        void SetEnv(Connection &conn, HttpRequest &req);    
        void PrintEnv();
};

#endif