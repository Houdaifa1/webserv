#include "../../includes/utils_hpp/ConfigUtils.hpp"
#include "../../includes/webserv.hpp"

bool is_method_allowed(const Location &loc, const std::string &method)
{
    for (size_t i = 0; i < loc.directives.size(); ++i)
    {
        if (loc.directives[i].name == "allowed_methods")
        {
            const std::vector<std::string> &args = loc.directives[i].args;
            for (size_t j = 0; j < args.size(); ++j)
            {
                if (args[j] == method)
                    return true;
            }
            return false;
        }
    }
    return true;
}

std::string get_directive_value(const Location &loc, const std::string &name)
{
    for (size_t i = 0; i < loc.directives.size(); ++i)
    {
        if (loc.directives[i].name == name && !loc.directives[i].args.empty())
        {
            return loc.directives[i].args[0];
        }
    }
    return "";
}


std::string resolve_upload_path(const Location &loc)
{
    std::string upload_store = get_directive_value(loc, "upload_store");
    if (!upload_store.empty())
    {
        normalize_path(upload_store);
        return upload_store;
    }
    else
    {
        std::string loc_root = loc.root;
        normalize_path(loc_root);
        return loc_root;
    }
}

std::string make_fullpath(const std::string &base, const std::string &req_path)
{
    if (req_path.empty())
        return base;

    std::string result = base;
    if (!result.empty())
    {
        if (result[result.size() - 1] != '/')
            result += '/';
    }
    size_t start;
    if (req_path[0] == '/')
        start = 1;
    else
        start = 0;
    result += req_path.substr(start);

    return result;
}
