#include "../../../includes/webserv.hpp"

bool is_number(const std::string &s, size_t &port_num)
{
    port_num = 0;
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
            return false;
        port_num = port_num * 10 + (s[i] - '0');
    }
    return true;
}

bool is_address(const std::string &s)
{
    int counter = 0;
    size_t byte = 0;
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.size(); i++)
    {
        byte = byte * 10 + (s[i] - '0');
        if (!isdigit(s[i]))
        {
            if (s[i] != '.')
                return false;
            if (byte > 255)
                return false;
            byte = 0;
            counter++;
        }
    }
    if (counter != 3)
        return false;
    return true;
}

void parse_listen(Directive &directive, std::string &path)
{
    size_t port_num = 0;
    if (directive.args.size() != 1)
        throw Parsererror(InvalidNumberArgs, directive.name, path, directive.position.line);

    size_t del_pos = directive.args[0].find(':');
    if (del_pos == std::string::npos)
    {
        if (!is_number(directive.args[0], port_num))
            throw Parsererror(InvalidArgument, directive.name, path, directive.position.line);
        if (!(port_num >= 1 && port_num <= 65535))
            throw Parsererror(InvalidPortNumber, directive.args[0], path, directive.position.line);
        std::string address = "0.0.0.0:";
        directive.args[0] = address + directive.args[0];
    }
    else
    {
        std::string address = directive.args[0].substr(0, del_pos);
        if (address.empty())
            throw Parsererror(MissingAddress, directive.args[0], path, directive.position.line);
        if (!is_address(address))
            throw Parsererror(InvalidAddress, address, path, directive.position.line);
        std::string port = directive.args[0].substr(del_pos + 1);
        if (!is_number(port, port_num))
            throw Parsererror(InvalidArgument, directive.name, path, directive.position.line);
        if (!(port_num >= 1 && port_num <= 65535))
            throw Parsererror(InvalidPortNumber, port, path, directive.position.line);
        directive.args[0] = address + ":" + port;
    }
}

// ":5050"
// 8080     10.0.0.0  10.0.0  shsdjfhsdjkf 1010101010