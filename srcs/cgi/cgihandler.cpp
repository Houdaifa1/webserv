#include "../../includes/cgi/cgihandler.hpp"
#include <sys/wait.h>

CgiHandler::CgiHandler(Connection &conn, HttpRequest &req) : name("cgi_path"),
    conn(conn), req(req), direct(conn.location.directives),
    fullpath(conn.location.root + req.get_correct_path()),
    cmd_set(false), environment(conn, req) {}

int CgiHandler::GetSize(){
    return (environment.GetEnv().size());
}

char* cpp_strdup(const char* s) {
    if (s == NULL) {
        return NULL;
    }
    size_t len = std::strlen(s) + 1;
    char* new_string = new char[len];
    std::strcpy(new_string, s);
    return new_string;
}

void CgiHandler::GetEnv(char **env) {
    std::vector<std::string> envv = environment.GetEnv();
    int size = envv.size();
    for (int i = 0; i < size; i++)
        env[i] = cpp_strdup(envv[i].c_str());
    env[size] = NULL;
}

int CgiHandler::GetCommands(std::string ext, std::vector<std::string> &commands){
    std::vector<std::string>::iterator it;
    std::string type, temp, temp2;
    size_t tmp;

    for(it = commands.begin(); it != commands.end(); ++it)
    {
        if (ext == ".py")
            type = "python3";
        else if (ext == ".sh")
            type = "bash";
        tmp = (*it).find(type);
        if (tmp != std::string::npos && std::strcmp(it->c_str() + tmp, type.c_str()) == 0)
        {
            std::cout << "I'M HERE:   1\n";
            if(*it == type){
                temp = "/usr/bin/" + type;
                cmnd = cpp_strdup(temp.c_str());
                return (1);
            }
            tmp = it->find_last_of("/");
            if (tmp != std::string::npos && access(it->c_str(), R_OK || X_OK) == 0){
                    temp = it->c_str() + tmp;
                    temp2 = "/" + type;
                    if(temp == temp2){
                        cmnd = cpp_strdup(it->c_str());
                        return (1);
                    }
            }
        }
    }
    return (0);
}

int CgiHandler::SetCommands(){
    ErrorHandler error_mesg(conn.location, conn);
    std::vector<Directive>::iterator it;

    size_t dot = fullpath.find_last_of('.');
    std::string ext = fullpath.substr(dot);

    args[2] = NULL;
    for(it = direct.begin(); it != direct.end(); ++it){
        if (it->name == this->name){
                if ((ext == ".py" && GetCommands(ext, it->args))
                    || (ext == ".sh" && GetCommands(ext, it->args))){
                    cmd_set = true;
                    std::cout << "IM CMND: " << cmnd << std::endl;
                    this->args[0] = cpp_strdup(cmnd); // copy without c_str() to lose const
                    this->args[1] = cpp_strdup(fullpath.c_str()); // copy without c_str() to lose const
                    this->args[2] = NULL;
                    break;
                }
                else 
                    return (0);
        }
    }
    (void)req;
    return (1);
}

bool CgiHandler::CheckFile(){
    ErrorHandler error_mesg(conn.location, conn);

    const std::string file = fullpath;
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
    char tmp[501];
    ErrorHandler error_msg(conn.location, conn);
    std::string buffer;
    char **env = new char*[GetSize() + 1];

    GetEnv(env);
    int parent_pid = fork();

    if (parent_pid == 0){
        int status;
        if (pipe(fd) == -1)
            return (1);

        pid_t pid = fork();

        if (pid == -1){
            close(fd[0]);
            close(fd[1]);
            return (1);
        }
        if (pid == 0){
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            execve(cmnd, args, env);
            error_msg.generate_error_response(500);
            close(fd[1]);
            std::exit(1);
        }
        else {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                close(fd[1]);
                if (WEXITSTATUS(status) == 0){
                int bytes = 1;
                while (bytes != 0){
                    bytes = read(fd[0], tmp, 500);
                    tmp[bytes] = '\0';
                    buffer += tmp;
                }
                std::stringstream header;
                header << "HTTP/1.0 200 Ok\r\n"
                    << "Content-Type: text/html\r\n"
                    << "Content-Length: " << buffer.size() << "\r\n"
                    << "Connection: close\r\n\r\n";
                std::string response = header.str() + buffer;
                send(conn.client_fd, response.c_str(), response.length(), 0);
                }
                close(fd[0]);
            }
        }
        delete[] cmnd;
        for (int i = 0; args[i]; i++){
            delete[] args[i];
        }
        for (int i = 0; i < GetSize(); ++i) {
            delete[] env[i];
        }
        exit(0);
    }
    delete[] cmnd;
    for (int i = 0; args[i]; i++){
        delete[] args[i];
    }
    for (int i = 0; i < GetSize(); ++i) {
        delete[] env[i];
    }
    delete[] env;
    return (0);
}
