#include "../../includes/cgi/cgihandler.hpp"
#include <sys/wait.h>

CgiHandler::CgiHandler(Connection &conn, HttpRequest &req) : name("cgi_path"),
    conn(conn), req(req), direct(conn.location.directives),
    fullpath(conn.location.root + req.get_correct_path()),
    environment(conn, req) {}

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
            if(*it == type){
                temp = "/usr/bin/" + type;
                command = temp;
                return (1);
            }
            tmp = it->find_last_of("/");
            if (tmp != std::string::npos && access(it->c_str(), R_OK || X_OK) == 0){
                    temp = it->c_str() + tmp;
                    temp2 = "/" + type;
                    if(temp == temp2){
                        command = *it;
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

    for(it = direct.begin(); it != direct.end(); ++it){
        if (it->name == this->name){
                if ((ext == ".py" && GetCommands(ext, it->args))
                    || (ext == ".sh" && GetCommands(ext, it->args))){
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
    std::vector<std::string> src = environment.GetEnv();
    std::vector<std::string>::iterator it;

    for (it = src.begin(); it != src.end(); ++it){
        dst.push_back((char *)it->c_str());
    }
    dst.push_back(NULL);
    char **env = dst.data();
    char *args[3] = {(char *)command.c_str(), (char *)fullpath.c_str(), NULL};

    int parent_pid = fork();

    if (parent_pid == -1)
        return (0);

    if (parent_pid == 0){
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1){
            error_msg.generate_error_response(500);
            std::exit(1);
        }

        pid_t pid = fork();

        if (pid == -1){
            close(pipe_in[0]);
            close(pipe_in[1]);
            close(pipe_out[0]);
            close(pipe_out[1]);
            error_msg.generate_error_response(500);
            std::exit(1);
        }
        if (pid == 0){
            close(pipe_in[1]);
            close(pipe_out[0]);
            dup2(pipe_in[0], STDIN_FILENO);
            dup2(pipe_out[1], STDOUT_FILENO);
            execve((char *)command.c_str(), args, env);
            error_msg.generate_error_response(500);
            std::exit(1);
        }
        else {
            close(pipe_in[0]);
            close(pipe_out[1]);
    
            if (conn.request.get_httpmethod() == "POST" && !conn.request.get_body().empty())
                write(pipe_in[1], conn.request.get_body().c_str(), conn.request.get_body().size());

            close(pipe_in[1]);

            time_t start_time = conn.last_activity;
            const int TIMEOUT_SECONDS = 10;
            pid_t wait_result;
            int status;
            struct timeval tv;

            while ((wait_result = waitpid(pid, &status, WNOHANG)) == 0) {
                time_t current_time = std::time(NULL);
                if (current_time - start_time > TIMEOUT_SECONDS) {
                    kill(pid, SIGKILL);
                    waitpid(pid, NULL, 0);
                    close(pipe_out[0]);
                    error_msg.generate_error_response(504);
                    std::exit(1);
                }
                tv.tv_sec = 0;
                tv.tv_usec = 100000;
                select(0, NULL, NULL, NULL, &tv);
            }

            std::string buffer;
            int bytes;
            while ((bytes = read(pipe_out[0], tmp, 500)) > 0)
            {
                tmp[bytes] = '\0';
                buffer += tmp;
            }
            close(pipe_out[0]);

            waitpid(pid, NULL, 0);
            std::stringstream header;
            header << "HTTP/1.0 200 Ok\r\n"
                << "Content-Type: text/html\r\n";
            std::size_t pos = buffer.find("Set-Cookie:", 0);
            size_t end = 0;
            if (pos == 0){
                for (end = pos; buffer[end] != '\n' ; end++)
                    ;
                std::string tmp = buffer.substr(pos, end);
                header << tmp << "\r\n";
                while (buffer[end] != '<')
                end++;
            }
            header << "Content-Length: " << buffer.size() - end << "\r\n"
                << "Connection: close\r\n\r\n";
            std::string response = header.str() + (buffer.c_str() + end);
            send(conn.client_fd, response.c_str(), response.length(), 0);
        }
        std::exit(0);
    }
    return (1);
}

