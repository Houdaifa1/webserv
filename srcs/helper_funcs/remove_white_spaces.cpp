#include "../../includes/webserv.hpp"

void ft_remove_whitespaces(std::string& s)
{
    size_t start = 0;
    while (start < s.size() && ((s[start] >= 9 && s[start] <= 13) || s[start] == ' '))
    {
        ++start;
    }
    size_t end = s.size();
    while (end > start && ((s[end - 1] >= 9 && s[end - 1] <= 13) || s[end - 1] == ' '))
    {
        --end;
    }
    s = s.substr(start, end - start);
}