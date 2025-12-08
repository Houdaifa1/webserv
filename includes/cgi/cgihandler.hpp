#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../webserv.hpp"
#include "Environment.hpp"

class CgiHandler {
    private:
        const std::string name;
        std::string command;
        Connection &conn;
        HttpRequest &req;
        std::vector<Directive> direct;
        const std::string fullpath;
        std::vector<char *> dst;
        int pipe_in[2];
        int pipe_out[2];
        int GetCommands(std::string ext, std::vector<std::string> &command);
    public:
        Environment environment;
        CgiHandler(Connection &conn, HttpRequest &req);
        int GetSize();
        int SetCommands();
        bool CheckFile();
        int ExecuteScript();
};

#endif