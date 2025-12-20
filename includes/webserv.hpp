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
# include "multiplexing_hpp/EventLoop_utils.hpp"
# include "signal.h"
# include "utils_hpp/logs.hpp"


enum RequestResult
{
    SUCCESS,
    INCOMPLETE,
    ERROR    
};

int             get_config(std::string file_name, Config  &config);
void            parse_listen(Directive &directive, std::string &path);
void            parse_root(Directive &directive, std::string &path);
void            parse_autoindex(Directive &directive, std::string &path);
void            parse_body_size(Directive &directive, std::string &path);
void            parse_index(Directive &directive, std::string &path);
void            parse_upload_store(Directive &directive, std::string &path);
void            parse_cgi_path(Directive &directive, std::string &path);
void            parse_allowed_methods(Directive &directive, std::string &path);
int             startserver(const Config &config);
std::string     intTo_String(int n);
RequestResult   parse_http_request(std::string& request, HttpRequest &req);
void            ft_remove_whitespaces(std::string& s);
void            parse_error_pages(Server &server, std::string &path);
void            normalize_path(std::string &path);
void            SignalHandler(int sig);
void            send_executed_cgi(int client_fd, const std::string &path);

# endif