# ifndef PARSER_HPP
# define PARSER_HPP

# include "Config.hpp"

class Parser
{

    private :

        std::vector<Token>  tokens;
        size_t              index;
        std::string         path;

    public :

        Parser(const std::vector<Token> &tokens, std::string path);
        int Parseall(Config &config);
        bool is_serever_block(Token &token);
        bool is_directive(Token &token, const std::string &where);
        bool is_known_directive(std::string &value, const std::string &where);
        Directive parse_directive();
        Server parse_server();
        Location parse_location();
        void validate_config(Config &config);
        void validate_server(Config &config, Server &server);
        void validate_location(Server &server, Location &location, int server_root_count);
        void validate_locations(Server &server, int server_root_count);
        void check_directive(Directive &directive, bool &contain_listen, int &contain_root, Config &config, std::string &global_root);

};



# endif