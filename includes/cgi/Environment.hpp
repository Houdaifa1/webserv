#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "../../includes/Libraries.hpp"
#include "../../includes/webserv.hpp"

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
