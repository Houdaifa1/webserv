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
    ErrorHandler error_mesg(conn.location, conn);
    std::vector<Directive>::iterator it;

    size_t dot = fullpath.find_last_of('.');
    std::string ext = fullpath.substr(dot);

    for(it = direct.begin(); it != direct.end(); ++it){
        if (it->name == this->name){
                if (it->args.size() > 2){
                    error_mesg.generate_error_response(403);
                    return;
                }
                if (ext == ".py")
                    command = it->args[0];
                else if(ext == ".sh")
                    command = it->args[1];
                this->args[0] = command.c_str();
                this->args[1] = fullpath.c_str();
                break;
        }
    }
}

bool CgiHandler::CheckFile(){
    ErrorHandler error_mesg(conn.location, conn);

    const std::string file = fullpath;
    std::cout << "THIS IS FILE: " << file << "\n"; 
    if (access(conn.location.root.c_str(), R_OK | X_OK) == -1)
    {
        error_mesg.generate_error_response(403);
        return (false);
    }
    if (access(file.c_str(), F_OK) == 0){
        if (access(file.c_str(), X_OK | R_OK))
        {
            error_mesg.generate_error_response(403);
            return (false);
        }
        return (true);
    }
    error_mesg.generate_error_response(404);
    return (false);
}

int CgiHandler::ExecuteScript() {
    return (1);
}