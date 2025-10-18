# ifndef WEBSERv_HPP
# define WEBSERv_HPP

# include "token_hpp/Tokenizer.hpp"
# include "parse_config_hpp/Parser.hpp" 
# include "core_hpp/ServerCore.hpp" 
# include "http_hpp/HttpRequest.hpp"
# include "multiplexing_hpp/EventLoop.hpp"
# include "http_hpp/HttpHandler.hpp"
# include "http_hpp/HttpResponse.hpp"
# include "http_hpp/ErrorHandler.hpp"
# include "utils_hpp/ConfigUtils.hpp"
# include "utils_hpp/PostUtils.hpp"
# include "utils_hpp/ResponseUtils.hpp"
# include "cgi/cgihandler.hpp"
# include "signal.h"

enum RequestResult
{
    SUCCESS,
    INCOMPLETE,
    ERROR    
};

int             get_config(char *file_name, Config  &config);
void            parse_listen(Directive &directive, std::string &path);
void            parse_root(Directive &directive, std::string &path);
void            parse_autoindex(Directive &directive, std::string &path);
int             startserver(const Config &config);
std::string     intTo_String(int n);
RequestResult   parse_http_request(std::string request, HttpRequest &req);
void            ft_remove_whitespaces(std::string& s);
void            parse_error_pages(Server &server, std::string &path);
void            normalize_path(std::string &path);
void            SignalHandler(int sig);

# endif