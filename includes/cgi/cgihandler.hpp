#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../webserv.hpp"
#include "Environment.hpp"

class CgiHandler {
    private:
        const std::string name;
        std::string command; // change to char *
        const char *args[3]; // lose the const
        Connection &conn;
        HttpRequest &req;
        std::vector<Directive> direct;
        const std::string fullpath;
        int fd[2];
    public:
        Environment environment;
        CgiHandler(Connection &conn, HttpRequest &req);
        int GetSize();
        void GetEnv(char **env);
        void SetCommands();
        bool CheckFile();
        int ExecuteScript();
};

#endif