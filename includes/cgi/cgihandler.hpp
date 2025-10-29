#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../webserv.hpp"
#include "Environment.hpp"

class CgiHandler {
    private:
        const std::string name;
        std::string command;
        const char *args[2];
        Connection &conn;
        HttpRequest &req;
        std::vector<Directive> direct;
        const std::string fullpath;
    public:
        Environment environment;
        CgiHandler(Connection &conn, HttpRequest &req);
        std::vector<std::string> GetEnv();
        void SetCommands();
        bool CheckFile();
        int ExecuteScript();
};

#endif