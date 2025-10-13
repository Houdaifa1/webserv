#ifndef CONFIGUTILS_HPP
#define CONFIGUTILS_HPP

#include "../webserv.hpp"
#include "../parse_config_hpp/Location.hpp"
# include "../http_hpp/HttpHandler.hpp"

bool is_method_allowed(const Location &loc, const std::string &method);

std::string get_directive_value(const Location &loc, const std::string &name);

std::string resolve_upload_path(const Location &loc);

std::string make_fullpath(const std::string &base, const std::string &req_path);

# endif 