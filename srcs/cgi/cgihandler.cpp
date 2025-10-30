#include "../../includes/cgi/cgihandler.hpp"
#include <sys/wait.h>
CgiHandler::CgiHandler(Connection &conn, HttpRequest &req) : name("cgi_path"), conn(conn), req(req), direct(conn.location.directives), fullpath(conn.location.root + req.get_correct_path()), environment(conn, req) {}

int CgiHandler::GetSize(){
    return (environment.GetEnv().size());
}

void CgiHandler::GetEnv(char **env) {
    std::vector<std::string> envv = environment.GetEnv();
    int size = envv.size();
    for (int i = 0; i < size; i++){
        env[i] = new char[envv[i].size() + 1]; 
        std::strncpy(env[i], envv[i].c_str(), envv[i].size());
        env[i][envv[i].size()] = '\0';
    }
    env[size] = NULL;
}

void CgiHandler::SetCommands(){
    ErrorHandler error_mesg(conn.location, conn);
    std::vector<Directive>::iterator it;

    size_t dot = fullpath.find_last_of('.');
    std::string ext = fullpath.substr(dot);

    args[2] = NULL;
    for(it = direct.begin(); it != direct.end(); ++it){
        if (it->name == this->name){
                if (it->args.size() > 2){
                    error_mesg.generate_error_response(403);
                    return;
                }
                if (ext == ".py")
                    command = it->args[0]; // copy without c_str() to lose const
                else if(ext == ".sh")
                    command = it->args[1]; // copy without c_str() to lose const
                this->args[0] = command.c_str(); // copy without c_str() to lose const
                this->args[1] = fullpath.c_str(); // copy without c_str() to lose const
                break;
        }
    }
    (void)req;
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
    char buffer[50];
    char **env = new char*[GetSize()];

    GetEnv(env);

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
        write(1, "1234567 ", 8);
        std::cout << "* " << fd[1] << " *" << "child finished";
        close(fd[1]);

        std::exit(1);
    }
    else {
        waitpid(pid, NULL, 0);
        close(fd[1]);
        int bytes = read(fd[0], buffer, 50);
        buffer[bytes] = '\0';
        std::cout << "This is buffer: " << buffer << std::endl;
        close(fd[0]);
    }
    // for (int i = 0; i < GetSize(); ++i) {
    //     delete[] env[i];
    //     env[i] = NULL;
    // }
    // delete[] env;
    return (0);
    // char buffer[50];
    // if (pipe(fd) == -1)
    //     return (1);

    // pid_t pid = fork();

    // if (pid == -1){
    //     close(fd[0]);
    //     close(fd[1]);
    //     return (1);
    // }

    // if (pid == 0){
    //     close(fd[0]);
    //     int tmp = dup(STDOUT_FILENO);
    //     dup2(tmp, fd[1]);
    //     write(1, "1234567\n", 8);
    //     std::cout << "* " << fd[1] << " *" << "child finished\n";
    //     dup2(fd[1], STDOUT_FILENO);
    //     close(fd[1]);
    //     close(tmp);
    //     std::exit(0);
    // }
    // else {
    //     waitpid(pid, NULL, 0);
    //     close(fd[1]);
    //     int bytes = read(fd[0], buffer, 8);
    //     buffer[bytes] = '\0';
    //     std::cout << "This is buffer: " << buffer << std::endl;
    //     close(fd[0]);
    // }
    // return (0);
}