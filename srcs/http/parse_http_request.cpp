# include "../../includes/webserv.hpp"

std::vector<std::string> split_by_two_spaces(const std::string& line)
{
    std::vector<std::string> result;

    size_t pos1 = line.find(' ');
    if (pos1 == std::string::npos) {
        result.push_back(line);
        return result;
    }
    result.push_back(line.substr(0, pos1));

    size_t pos2 = line.find(' ', pos1 + 1);
    if (pos2 == std::string::npos)
    {
        result.push_back(line.substr(pos1 + 1));
        return result;
    }

    result.push_back(line.substr(pos1 + 1, pos2 - (pos1 + 1)));
    result.push_back(line.substr(pos2 + 1));

    return result;
}
int countSpaces(const std::string& str)
{
    int count = 0;
    size_t i = 0;
    char c;
    while (i < str.size())
    {
        c = str[i];
        if (c == ' ')
            count++;
        i++;
    }
    return count;
}
bool parse_request_line(std::string &request_line, HttpRequest &req)
{
    std::vector<std::string>    words;
    words = split_by_two_spaces(request_line);

    if (countSpaces(request_line) != 2)
        return false;
    if (words.size() != 3)
        return false;
    if (words[0].empty() || words[1].empty() || words[2].empty())
        return false;
    if (words[1][0] != '/')
        return false;
    if (words[2] != "HTTP/1.1" && words[2] != "HTTP/1.0")
        return false;
    req.set_httpmethod(words[0]);
    req.set_requestpath(words[1]);
    req.set_httpversion(words[2]);
    return true;
}


bool parse_headers(std::string &request, size_t &index, HttpRequest &req)
{
    size_t end = request.find("\r\n", index);
    if (end == std::string::npos)
        return false;
    std::string line = request.substr(index, end - index);
    while (!line.empty())
    {
        size_t sep = line.find(':');
        if (sep == std::string::npos)
            return false;
        std::string key = line.substr(0, sep);
        std::string value = line.substr(sep + 1);
        ft_remove_whitespaces(key);
        ft_remove_whitespaces(value);
        if (key.empty())
            return false;
        req.set_headers(key, value);
        size_t pos = value.find(":");
        if (key == "Host")
        {
            req.set_host_domain(value.substr(0, pos));
            req.set_host_port(value.substr(pos + 1));
        }
        index = end + 2;
        end = request.find("\r\n", index);
        if (end == std::string::npos)
            return false;
        line = request.substr(index, end - index);
    }
    return true;
}
RequestResult parse_body(std::string &request, size_t &index, HttpRequest &req)
{
    std::string key = "Content-Length";

    std::map<std::string, std::string> headers = req.get_headers();
    std::map<std::string, std::string>::iterator it = headers.find(key);

    if (it == headers.end())
    {
        return SUCCESS;
    }
    std::string length = it->second;
    size_t body_length = (size_t)atoi(length.c_str());
    if (body_length == 0 && length != "0")
        return ERROR;
    if (request.size() - index < body_length)
        return INCOMPLETE;

    req.set_body(request.substr(index, body_length));
    return SUCCESS;
}

RequestResult parse_http_request(std::string& request, HttpRequest &req)
{
    size_t index = request.find("\r\n") ;
    if (index == std::string::npos)
        return ERROR;
    std::string request_line = request.substr(0, index);
    index += 2;
    if (!parse_request_line(request_line, req))
        return (ERROR);
         
    if (!parse_headers(request, index, req))
        return ERROR;
    index += 2;
    return(parse_body(request, index, req));

}

