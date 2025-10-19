#include "../../includes/utils_hpp/PostUtils.hpp"

bool dir_exists(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return false;
}


bool is_writable_dir(const std::string &path)
{
    return access(path.c_str(), W_OK) == 0;
}

std::string get_parent_dir(const std::string &path)
{
    size_t slash = path.find_last_of('/');
    if (slash == std::string::npos || slash == 0)
        return "";
    return path.substr(0, slash);
}


std::string get_header_value(const HttpRequest &req, const std::string &name)
{
    std::map<std::string, std::string> headers = req.get_headers();
    std::map<std::string, std::string>::iterator it = headers.find(name);
    if (it != headers.end())
        return it->second;
    else
        return "";
}

size_t  parse_size_vlue(const std::string &val)
{
    if (val.empty())
        return 0;
    
    char    last = val[val.size() - 1];
    size_t  num = std::atoi(val.c_str());
    
    if (last == 'K' || last == 'k')
        num *= 1024;
    else if (last == 'M' || last == 'm')
        num *= 1024 * 1024;
    else if (last == 'G' || last == 'g')
        num *= 1024 * 1024 * 1024;
    
    return num;
}

size_t  get_client_max_body_size(const Server &server)
{
    for (size_t i = 0; i < server.directives.size(); ++i)
    {
        if (server.directives[i].name == "client_max_body_size" && !server.directives[i].args.empty())
        {
            return parse_size_vlue(server.directives[i].args[0]);
        }
    }
    return 0;
}

bool file_exists(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
        return S_ISREG(st.st_mode);
    return false;
}

bool is_writable_file(const std::string &path)
{
    if (access(path.c_str(), F_OK) != 0)
        return false;
    if (access(path.c_str(), W_OK) == 0)
        return true;
    return false;
}
