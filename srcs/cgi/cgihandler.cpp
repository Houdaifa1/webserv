#include "../../includes/cgi/cgihandler.hpp"

CgiHandler::CgiHandler() {}

void CgiHandler::SetHeader(std::vector<std::string> &env, HttpRequest &req)
{
    std::map<std::string, std::string> headers = req.get_headers();
    std::vector<std::string> index = {"Content-Type", "Content-Length", "Host"};
    std::vector<std::string> var = {"CONTENT_TYPE", "CONTENT_LENGTH", "SERVER_NAME", "SERVER_PORT"};
    for (int i = 0; i < 3; i++)
    {
        if (headers.find(index[i]) != headers.end())
        {
            if (i == 2)
            {
                std::string::size_type pos = headers[index[i]].find(':');

                std::string name = (pos != std::string::npos) ? headers[index[i]].substr(0, pos) : headers[index[i]];
                std::string port = (pos != std::string::npos) ? headers[index[i]].substr(pos + 1) : "";

                env.push_back(var[i] + name);
                env.push_back(var[i + 1] + port);
            }
            else
                env.push_back(var[i] + headers[index[i]]);
        }
        else
        {
            env.push_back(var[i]);
            if (i == 2)
                env.push_back(var[i + 1]);
        }
    }
}

void CgiHandler::SetEnv(Connection &conn, HttpRequest &req) {
    SetHeader(env, req);
    env.push_back("REQUEST_METHOD=" + req.get_httpmethod());
    env.push_back("SERVER_PROTOCOL=" + req.get_httpversion());
    env.push_back("REDIRECT_STATUS=200");
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("QUERY_STRING=" + req.get_query());
    env.push_back("SCRIPT_NAME=/cgi-bin/" + req.get_correct_path());
    env.push_back("PATH_TRANSLATED=" + (conn.location.root + req.get_correct_path()));
}