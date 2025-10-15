#include "../../includes/utils_hpp/FileUtils.hpp"

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