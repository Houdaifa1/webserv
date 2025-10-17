# include "../../../includes/webserv.hpp"

void check_error_page(Directive directive, std::string &path)
{
    size_t end = directive.args.size();
    if (directive.args.size() < 2)
    {
        throw Parsererror(InvalidNumberArgs2, "error_page", path, directive.position.line);
    }
    
    for (size_t i = 0; i < end - 1; i++)
    {
        size_t j = 0;
        while (j < directive.args[i].size())
        {
            if (!isdigit(directive.args[i][j]))
            {
                throw Parsererror(ErrorCode, "error_page", path, directive.position.line);
            }
        }
    }
    normalize_path(directive.args[end - 1]);
    if (directive.args[end - 1][0] != '/')
        throw Parsererror(NotPath, directive.args[end - 1], path, directive.position.line);
}

void parse_error_pages(Server &server, std::string &path)
{
    for (size_t i = 0; i < server.directives.size(); i++)
    {
        if (server.directives[i].name == "error_page")
        {
            check_error_page(server.directives[i], path);x
        }

    }


}
