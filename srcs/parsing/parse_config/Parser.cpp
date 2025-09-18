#include "../../../includes/webserv.hpp"

Parser::Parser(const std::vector<Token> &tokens, std::string path) : tokens(tokens),
                                                                     index(0), path(path) {}



bool Parser::is_known_directive(std::string &value)
{
    const char *directives[] = {
        "listen", "error_page", "client_max_body_size", "allowed_methods", "return",
        "redirect", "root", "autoindex", "index", "upload_store", "cgi_pass"};
    for (int i = 0; i < sizeof(directives); i++)
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
    bool server = false;

    if (token.value == "server")
    {
        server = true;
        // handlle server
    }
    return server;
}

Directive Parser::parse_directive(const std::vector<Token> &tokens, size_t &index)
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
            if (tokens[index].value != ";")
                throw Parsererror(DirectiveNotTerminated, directive.name, path, line);
        }
        directive.args[index] = tokens[index].value;
    }

}

int Parser::Parseall(Config &config)
{
    try
    {
        while (index < tokens.size())
        {
            if (is_serever_block(tokens[index]))
            {
                // parse_server
                continue;
            }
            if (is_directive(tokens[index]))
            {
                Directive new_directive;
                new_directive = parse_directive(tokens, index);
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