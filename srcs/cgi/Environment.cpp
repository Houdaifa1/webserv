#include "../../includes/cgi/Environment.hpp"

Environment::Environment(Connection &conn, HttpRequest &req) : conn(conn), req(req) {}

void Environment::SetHeaders()
{
    std::map<std::string, std::string> headers = req.get_headers();
    std::string index[] = {"Content-Type", "Content-Length"};
    std::string var[] = {"CONTENT_TYPE=", "CONTENT_LENGTH="};
    for (int i = 0; i < 2; i++)
    {
        if (headers.find(index[i]) != headers.end())

                env.push_back(var[i] + headers[index[i]]);
        else
            env.push_back(var[i]);
    }
}

void Environment::SetHttpHeaders(){
    std::map<std::string, std::string> headers;

    headers = req.get_headers();
    std::map<std::string, std::string>::iterator iter;

    for (iter = headers.begin(); iter != headers.end(); ++iter){
        std::string var = iter->first;
        std::string val = iter->second;

        if (var == "Content-Type" || var == "Content-Length" || var == "Host")
            continue;
        
        for (unsigned long i = 0; i < var.size(); i++){
            if (var[i] == '-')
                var[i] = '_';
            else
                var[i] = std::toupper(var[i]);
        }

        env.push_back("HTTP_" + var + '=' + val);
    }
}

void Environment::SetEnv() {
    std::vector<Directive>::iterator iter;
    std::vector<std::string>::iterator it;

    SetHeaders();
    SetHttpHeaders();
    env.push_back("REQUEST_METHOD=" + req.get_httpmethod());
    env.push_back("SERVER_PROTOCOL=" + req.get_httpversion());
    env.push_back("SERVER_NAME=" + req.get_host_domain());
    env.push_back("SERVER_PORT=" + req.get_host_port());
    env.push_back("REDIRECT_STATUS=200");
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("QUERY_STRING=" + req.get_query());
    env.push_back("SCRIPT_NAME=" + conn.location.path + req.get_correct_path());
    // conn.location.path should be fixed !!!!
    env.push_back("PATH_TRANSLATED=" + conn.location.root + req.get_correct_path());
    // env.push_back("****TEST_ROOT: " + conn.location.path);
    env.push_back("REMOTE_ADDR=" + conn.client_ip);
    std::cout << "******* Directives ********\n";
    // int i = 1;

    // for(iter = conn.location.directives.begin(); iter != conn.location.directives.end(); ++iter){
    //     std::cout << "Directive: No " << ++i << std::endl;
    //     std::cout << "Name: " << iter->name << std::endl;
    //     std::cout << "Args: ";
    //     for(it = iter->args.begin(); it != iter->args.end(); ++it){
    //         std::cout << *it << " - ";
    //     }
    //     std::cout << "\n";
    // }
    // for(int i = 0; i < size; i++){
    //     std::cout << "Directive: No "<< std::endl;
    //     std::cout << "Name: " << conn.location.directives[i].name << std::endl;
    //     std::cout << "Args: ";
    //     for(it = env.begin(); it != env.end(); ++it){
    //         std::cout << *it << " - ";
    //     }
    //     std::cout << "\n";
    // }
}

void Environment::PrintEnv(){
    std::vector<std::string>::iterator iter;

    for(iter = env.begin(); iter != env.end(); ++iter){
        std::cout << *iter << std::endl;
    }
}

std::vector<std::string> Environment::GetEnv() {
    return env;
}