#include "../../../includes/webserv.hpp"

Parser::Parser(const std::vector<Token> &tokens, std::string path) : tokens(tokens),
                                                                     index(0), path(path) {}



bool Parser::is_known_directive(std::string &value)
{
    const char *directives[] = {
        "listen", "error_log", "worker_processes" ,"client_max_body_size", "allowed_methods", "return",
        "redirect", "root","error_page", "autoindex", "index", "server_name", "cgi_path", "upload_store"};
    
    for (size_t i = 0; i < sizeof(directives)/sizeof(directives[0]); i++)
    {
        if (value == directives[i])
            return true;
           
    }
    return false;
}

bool Parser::is_directive(Token &token)
{
    bool directive = false;
    if ((token.type == IDENTIFIER || token.type == STRING) &&
        is_known_directive(token.value))
    {
        return true;
    }
    else
    {
        throw Parsererror(UnknownDirective, token.value, path, token.line);
    }
    return directive;
}

bool Parser::is_serever_block(Token &token)
{
    if (token.value == "server")
    {
        return true;
    }
    return false;
}

Directive Parser::parse_directive()
{
    Directive directive;

    directive.name = tokens[index].value;
    int line = tokens[index].line;
    int col = tokens[index].col;
    directive.position.line = line;
    directive.position.col = col;
    index++;
    while (index < tokens.size())
    {
          
        if (tokens[index].type == SYMBOL)
        {
            
            if (tokens[index].value == ";")
            {
                index++;
                return directive;
            }
            else
                throw Parsererror(DirectiveNotTerminated, directive.name, path, line);
        }
        if (tokens[index].type == COMMENT)
        {
            index++;
            continue;
        }
        directive.args.push_back(tokens[index].value);
        index++;
    }
    throw Parsererror(DirectiveNotTerminated, directive.name, path, line);
}

bool is_path_in_quotes(std::string s)
{
    size_t i = 0;
    while (i < s.size() && ((s[i] >= 9 && s[i] <= 13) || s[i] == ' '))
    {
        i++;
    }
    s.erase(0, i);
    if (s[0] == '/')
        return true;
    return false;
}

Location Parser::parse_location()
{
    Location location;

    int col = tokens[index].col;
    int line = tokens[index].line;
    location.position.line = line;
    location.position.col = col;
    index++;
    if (index >= tokens.size())
        throw Parsererror(UnexpectedEOF, "", path, line);
    if (!is_path_in_quotes(tokens[index].value))
    {
        if (tokens[index].type != PATH)
            throw Parsererror(ExpectedPath, tokens[index].value, path, tokens[index].line);
    }
    location.path = tokens[index].value;
    index++;
    if (index >= tokens.size())
        throw Parsererror(UnexpectedEOF, "", path, line);
    if (tokens[index].value != "{")
        throw Parsererror(ExpectBlockStarter, tokens[index].value, path, line);
    index++;
    while (index < tokens.size())
    {
        if (tokens[index].type == SYMBOL)
        {
            if (tokens[index].value == "}")
            {
                index++;
                return location;
            }
            throw Parsererror(UnexpectedSymbol, tokens[index].value, path, tokens[index].line);
        }
        if (tokens[index].type == COMMENT)
        {
            index++;
            continue;
        }
        if (tokens[index].value == "location")
            throw Parsererror(NestedBlocks, tokens[index].value, path, tokens[index].line);
        if (tokens[index].value == "server")
            throw Parsererror(TopBolockLow, tokens[index].value, path, tokens[index].line);
        if (is_directive(tokens[index]))
        {
            Directive new_directive;
            new_directive = parse_directive();
            location.directives.push_back(new_directive);
            continue;
        }
    }
     throw Parsererror(UnexpectedEOFend, "", path, tokens[index - 1].line + 1);
}

Server Parser::parse_server()
{
    Server server;

    int col = tokens[index].col;
    int line = tokens[index].line;
    server.position.col = col;
    server.position.line = line;
    index++;
    if (index >= tokens.size())
        throw Parsererror(UnexpectedEOF, "", path, line);
    if (tokens[index].value != "{")
        throw Parsererror(ExpectBlockStarter, tokens[index].value, path, tokens[index].line);
    index++;
    while (index < tokens.size())
    {
        if (tokens[index].type == SYMBOL)
        {
            if (tokens[index].value == "}")
            {
                index++;
                return server;
            }
            throw Parsererror(UnexpectedSymbol, tokens[index].value,path, tokens[index].line);
        }
        if (tokens[index].type == COMMENT)
        {
            index++;
            continue;
        }
        if (tokens[index].value == "location")
        {
            Location new_location;
            new_location = parse_location();
            server.locations.push_back(new_location);
            continue;
        }
        if (tokens[index].value == "server")
            throw Parsererror(NestedBlocks, tokens[index].value,path, tokens[index].line);
        if (is_directive(tokens[index]))
        {
            Directive new_directive;
            new_directive = parse_directive();
            server.directives.push_back(new_directive);
            continue;
        }
    }
    throw Parsererror(UnexpectedEOFend, "", path, tokens[index - 1].line + 1);
}

int Parser::Parseall(Config &config)
{
    try
    {
        while (index < tokens.size())
        {
           
            if (tokens[index].type == COMMENT)
            {
                index++;
                continue;
            }
            if (is_serever_block(tokens[index]))
            {
                Server new_server;
                new_server = parse_server();
                config.servers.push_back(new_server);
                continue;
            }
            if (is_directive(tokens[index]))
            {
                Directive new_directive;
                new_directive = parse_directive();
                config.globals.push_back(new_directive);
                continue;
            }
        }
    }
    catch (Parsererror &error)
    {
        std::cerr << error.mesg;
        return 1;
    }

    return 0;
}