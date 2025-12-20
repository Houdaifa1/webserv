#include "../../../includes/webserv.hpp"

Parser::Parser(const std::vector<Token> &tokens, std::string path) : tokens(tokens),
                                                                     index(0), path(path) {}

bool Parser::is_known_directive(std::string &value, const std::string &where)
{
    static const std::string location_directives[] = {
        "allowed_methods",
        "root",
        "error_page",
        "autoindex",
        "index",
        "cgi_path",
        "upload_store"};

    static const std::string server_directives[] = {
        "listen",
        "root",
        "client_max_body_size",
        "error_page"};

    const std::string *arr = NULL;
    size_t size = 0;

    if (where == "location")
    {
        arr = location_directives;
        size = sizeof(location_directives) / sizeof(location_directives[0]);
    }
    else
    {
        arr = server_directives;
        size = sizeof(server_directives) / sizeof(server_directives[0]);
    }

    for (size_t i = 0; i < size; i++)
    {
        if (value == arr[i])
            return true;
    }

    return false;
}

bool Parser::is_directive(Token &token, const std::string &where)
{
    bool directive = false;
    if ((token.type == IDENTIFIER || token.type == STRING) &&
        is_known_directive(token.value, where))
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
        if (is_directive(tokens[index], "location"))
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
            throw Parsererror(UnexpectedSymbol, tokens[index].value, path, tokens[index].line);
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
            throw Parsererror(NestedBlocks, tokens[index].value, path, tokens[index].line);
        if (is_directive(tokens[index], "server"))
        {
            Directive new_directive;
            new_directive = parse_directive();
            server.directives.push_back(new_directive);
            continue;
        }
    }
    throw Parsererror(UnexpectedEOFend, "", path, tokens[index - 1].line + 1);
}

void Parser::check_directive(Directive &directive, bool &contain_listen, int &contain_root, Config &config, std::string &global_root)
{
    if (directive.name == "listen")
    {
        parse_listen(directive, path);
        std::pair<std::string, int> pair = std::make_pair(directive.args[0], atoi(directive.args[1].c_str()));
        config.pairs.push_back(pair);
        contain_listen = true;
    }
    if (directive.name == "root")
    {
        parse_root(directive, path);
        global_root = directive.args[0];
        contain_root++;
    }
    if (directive.name == "client_max_body_size")
    {
        parse_body_size(directive, path);
    }
}

void Parser::validate_location(Server &server, Location &location, int server_root_count)
{
    int root_count = 0;
    int autoindex_count = 0;
    int allowed_methods_count = 0;
    int index_count = 0;
    int upload_store_count = 0;
    int cgi_path_count = 0;

    location.root = server.global_root;
    location.autoindex = "none";

    for (size_t i = 0; i < location.directives.size(); ++i)
    {
        Directive &d = location.directives[i];

        if (d.name == "allowed_methods")
        {
            ++allowed_methods_count;
            if (allowed_methods_count > 1)
                throw Parsererror( DuplicateAllowedMethods, "location", path, d.position.line);
            parse_allowed_methods(d, path);
        }
        else if (d.name == "upload_store")
        {
            ++upload_store_count;
            if (upload_store_count > 1)
                throw Parsererror(DuplicateUploadStore, "location", path, d.position.line);
            parse_upload_store(d, path);
        }
        else if (d.name == "index")
        {
            ++index_count;
            if (index_count > 1)
                throw Parsererror(DuplicateIndex, "location", path, d.position.line);
            parse_index(d, path);
        }
        else if (d.name == "root")
        {
            ++root_count;
            if (root_count > 1)
                throw Parsererror(DuplicateRoot, "location", path, d.position.line);
            parse_root(d, path);
            location.root = d.args[0];
        }
        else if (d.name == "autoindex")
        {
            ++autoindex_count;
            if (autoindex_count > 1)
                throw Parsererror(DuplicateAutoindex, "location", path, d.position.line);
            parse_autoindex(d, path);
            location.autoindex = d.args[0];
        }
        else if (d.name == "cgi_path")
        {
            ++cgi_path_count;
            if (cgi_path_count > 1)
                throw Parsererror(DuplicateCgiPath, "location", path, d.position.line);
            parse_cgi_path(d, path );
        }
    }

    if (root_count == 0 && server_root_count != 1)
        throw Parsererror(MissingRoot, "", path, location.position.line);
}

void Parser::validate_locations(Server &server, int server_root_count)
{
    for (size_t i = 0; i < server.locations.size(); ++i)
    {
        validate_location(server, server.locations[i], server_root_count);
    }
}


void Parser::validate_server(Config &config, Server &server)
{
    bool has_listen = false;
    int root_count = 0;
    int client_body_size_count = 0;

    for (size_t i = 0; i < server.directives.size(); ++i)
    {
        Directive &d = server.directives[i];

        if (d.name == "client_max_body_size")
        {
            ++client_body_size_count;
            if (client_body_size_count > 1)
                throw Parsererror(DuplicateClientMaxBodySize, "server", path, d.position.line);
        }
        check_directive(d, has_listen, root_count, config, server.global_root);
    }

    if (!has_listen)
        throw Parsererror(MissingListen, "", path, 0);

    if (root_count > 1)
        throw Parsererror(DuplicateRoot, "server", path, 0);

    validate_locations(server, root_count);
    parse_error_pages(server, path);
}

void Parser::validate_config(Config &config)
{
    if (config.servers.empty())
        throw Parsererror(NoServerFound, "", path, 0);

    if (config.globals.size() >= 1)
        throw Parsererror(GlobalDirective, config.globals[0].name, path, config.globals[0].position.line);

    for (size_t i = 0; i < config.servers.size(); ++i)
        validate_server(config, config.servers[i]);
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
            if (is_directive(tokens[index], "Global"))
            {
                Directive new_directive;
                new_directive = parse_directive();
                config.globals.push_back(new_directive);
                continue;
            }
        }
        validate_config(config);
    }
    catch (Parsererror &error)
    {
        std::cerr << error.mesg;
        return 1;
    }
    return 0;
}