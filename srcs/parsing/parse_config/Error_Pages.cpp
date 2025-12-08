# include "../../../includes/webserv.hpp"

void check_error_page(Directive &directive, std::string &path)
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
            j++;
        }
    }
    std::string full_path = "/" + directive.args[end - 1];
    directive.args[end - 1] = full_path;
    normalize_path(directive.args[end - 1]);
}

void insert_error_pages_in_map(Directive &directive, std::map<std::string, std::string> &map)
{
    size_t last = directive.args.size();
    std::string path = directive.args[last - 1]; 
    for (size_t j = 0; j < last - 1; j++)
    {
       map[directive.args[j]] = path;
    }
}

void parse_error_pages(Server &server, std::string &path)
{
    std::map<std::string, std::string> global_errors;
    for (size_t i = 0; i < server.directives.size(); i++)
    {
        if (server.directives[i].name == "error_page")
        {
            check_error_page(server.directives[i], path);
            insert_error_pages_in_map(server.directives[i], global_errors);
        }
    }
        
    for (size_t i = 0; i < server.locations.size(); i++)
    {
        server.locations[i].errors = global_errors;
        for (size_t j = 0; j < server.locations[i].directives.size(); j++)
        {
            if (server.locations[i].directives[j].name == "error_page")
            {
                check_error_page(server.locations[i].directives[j], path);
                insert_error_pages_in_map(server.locations[i].directives[j], server.locations[i].errors);
            }
        }
    }
}
