#include "../../includes/cgi/cgihandler.hpp"

CgiHandler::CgiHandler(Connection &conn, HttpRequest &req) : name("cgi-bin"), environment(conn, req) {}

std::vector<std::string> CgiHandler::GetEnv() {
    std::vector<std::string> envv = environment.GetEnv();
    int size = envv.size();
    const char *env[size + 1];
    for (int i = 0; i < size; i++)
        env[i] = envv[i].c_str();
    env[size] = NULL;
    std::cout << "~~~~~~~~~~~CHAR ** ENV~~~~~~~~~~~~~\n";
    for (int i = 0; env[i]; i++)
        std::cout << env[i] << std::endl;
    std::cout << "~~~~~~~~~~~ END ~~~~~~~~~~~~~\n";
    return (environment.GetEnv());
}
