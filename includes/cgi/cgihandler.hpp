#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../webserv.hpp"
#include "Environment.hpp"

class CgiHandler {
    private:
        const std::string name;
        char *cmnd;
        char *args[3]; // lose the const
        Connection &conn;
        HttpRequest &req;
        std::vector<Directive> direct;
        const std::string fullpath;
        bool cmd_set;
        // bool is_path;
        int fd[2];
        int GetCommands(std::string ext, std::vector<std::string> &command);
    public:
        Environment environment;
        CgiHandler(Connection &conn, HttpRequest &req);
        int GetSize();
        void GetEnv(char **env);
        int SetCommands();
        bool CheckFile();
        int ExecuteScript();
};

#endif