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
    int byte = 0;
    int digits = 0;

    for (size_t i = 0; i < s.size(); i++)
    {
        char c = s[i];
        if (isdigit(c))
        {
            byte = byte * 10 + (c - '0');
            digits++;
            if (byte > 255)
                return false;
            if (digits > 3)
                return false;
        }
        else if (c == '.')
        {
            if (digits == 0)
                return false;
            counter++;
            byte = 0;
            digits = 0;
        }
        else
            return false;
    }
    if (digits == 0 || byte > 255 || counter != 3)
        return false;
    return true;
}

void parse_listen(Directive &directive, std::string &path)
{
    size_t port_num = 0;
    std::string port;
    if (directive.args.size() != 1)
        throw Parsererror(InvalidNumberArgs, directive.name, path, directive.position.line);

    size_t del_pos = directive.args[0].find(':');
    if (del_pos == std::string::npos)
    {
        if (!is_number(directive.args[0], port_num))
            throw Parsererror(InvalidArgument, directive.name, path, directive.position.line);
        if (!(port_num >= 1 && port_num <= 65535))
            throw Parsererror(InvalidPortNumber, directive.args[0], path, directive.position.line);
        std::string address = "0.0.0.0";
        port = directive.args[0];
        directive.args[0] = address;
        directive.args.push_back(port);
    }
    else
    {
        std::string address = directive.args[0].substr(0, del_pos);
        if (address.empty())
            throw Parsererror(MissingAddress, directive.args[0], path, directive.position.line);
        if (!is_address(address))
            throw Parsererror(InvalidAddress, address, path, directive.position.line);
        port = directive.args[0].substr(del_pos + 1);
        if (!is_number(port, port_num))
            throw Parsererror(InvalidArgument, directive.name, path, directive.position.line);
        if (!(port_num >= 1 && port_num <= 65535))
            throw Parsererror(InvalidPortNumber, port, path, directive.position.line);
        directive.args[0] = address;
        directive.args.push_back(port);
    }
}



void parse_root(Directive &directive, std::string &path)
{
    if (directive.args.size() != 1)
        throw Parsererror(InvalidNumberArgs, directive.name, path, directive.position.line);
}

void parse_autoindex(Directive &directive, std::string &path)
{
    if (directive.args.size() != 1)
        throw Parsererror(InvalidNumberArgs, directive.name, path, directive.position.line);
    if (directive.args[0] != "on" && directive.args[0] != "off")
        throw Parsererror(OnOffexpected, directive.name, path, directive.position.line);
}


void parse_body_size(Directive &directive, std::string &path)
{
    if (directive.args.size() != 1)
        throw Parsererror(InvalidNumberArgs, directive.name, path, directive.position.line);
}

void parse_upload_store(Directive &directive, std::string &path)
{
    if (directive.args.size() != 1)
        throw Parsererror(InvalidNumberArgs, directive.name, path, directive.position.line);
}

void parse_index(Directive &directive, std::string &path)
{
    if (directive.args.size() < 1)
        throw Parsererror(InvalidNumberArgsCgi, directive.name, path, directive.position.line);
}

void parse_cgi_path(Directive &directive, std::string &path)
{
    if (directive.args.size() < 1)
        throw Parsererror(InvalidNumberArgsCgi, directive.name, path, directive.position.line);
}


void parse_allowed_methods(Directive &directive, std::string &path)
{
    if (directive.args.size() > 3)
        throw Parsererror(InvalidNumberArgs, directive.name, path, directive.position.line);
    for (size_t i = 0; i < directive.args.size(); i++)
    {
        if (directive.args[i] != "GET" && directive.args[i] != "POST"
        && directive.args[i] != "DELETE")
            throw Parsererror(InvalidMethodArgs, directive.name, path, directive.position.line);
    }
}