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
    for (char c : str)
    {
        if (c == ' ')
            count++;
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
    if (words[0] != "GET" && words[0] != "POST" && words[0] != "DELETE")
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


    std::map<std::string, std::string>::iterator it = req.get_headers().find(key);

    std::string length = it->second;
    if (it == req.get_headers().end())
        return RequestResult::SUCCESS;

    size_t body_length = (size_t)atoi(length.c_str());
    if (body_length == 0 && length != "0")
        return RequestResult::ERROR;
    if (request.size() - index < body_length)
        return RequestResult::INCOMPLETE;

    req.set_body(request.substr(index, body_length));
    return RequestResult::SUCCESS;
}

RequestResult parse_http_request(std::string request, HttpRequest &req)
{
    size_t index = request.find("\r\n") ;
    if (index == std::string::npos)
        return RequestResult::ERROR;
    std::string request_line = request.substr(0, index);
    index += 2;
    if (!parse_request_line(request_line, req))
        return (RequestResult::ERROR);
    if (!parse_headers(request, index, req))
        return RequestResult::ERROR;
    index += 2;
    return(parse_body(request, index, req));
}



// int main ()
// {

//     std::string request = "GET /index.html HTTP/1.1\r\n"
//                       "Host: www.example.com\r\n"
//                       "Connection: keep-alive\r\n"
//                       "Content-Length: 0\r\n"
//                       "\r\n\r\n"
//                       "Hello";
//     RequestResult result = parse_http_request(request,req);
//     std::cout << "Method : " << req.get_httpmethod() << std::endl;
//     std::cout << "Path : " << req.get_requestpath() << std::endl;
//     std::cout << "Version : " << req.get_httpversion() << std::endl;
//     std::map<std::string, std::string> map = req.get_headers();
//     std::cout << "\n    Headers\n" <<  std::endl;
//     for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
//     {
//         std::cout << it->first << "   :   " <<  it->second << std::endl;
//     }
//     std::cout << "\n    Body\n" << std::endl << req.get_body() << std::endl;
// }