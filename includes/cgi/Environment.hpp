#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "../Libraries.hpp"
#include "../webserv.hpp"

class Environment {
    private:
        std::vector<std::string> env;
        Connection &conn;
        HttpRequest &req;
        void SetHeaders();
        void SetHttpHeaders();
    public:
        Environment(Connection &conn, HttpRequest &req);
        void SetEnv();   
        void PrintEnv();
        std::vector<std::string> GetEnv();
};

#endif
