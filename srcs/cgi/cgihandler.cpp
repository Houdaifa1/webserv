#include "../../includes/cgi/cgihandler.hpp"

CgiHandler::CgiHandler(Connection &conn, HttpRequest &req) : name("cgi_path"), conn(conn), req(req), direct(conn.location.directives), fullpath(conn.location.root + req.get_correct_path()), environment(conn, req) {}

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

void CgiHandler::SetCommands(){
    std::vector<Directive>::iterator it;

    for(it = direct.begin(); it != direct.end(); ++it){
        if (it->name == this->name){
            for (unsigned long j = 0; j < it->args.size(); j++)
                std::cout << "* " << it->args[j] << "\n";
        }
    }
    (void)conn;
    (void)req;
}

bool CgiHandler::CheckFile(const std::string file){
    struct stat buffer;

    if (stat(file.c_str(), &buffer) == 0){
        if (S_ISDIR(buffer.st_mode))
            return (false);
        return (true);
    }
    return (false);
}

int CgiHandler::ExecuteScript() {
    if (!CheckFile(fullpath)){
        return (1);
    }
    return (0);
}