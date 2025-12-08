#ifndef POSTUTILS_HPP
#define POSTUTILS_HPP

# include "../Libraries.hpp"
#include    "../../includes/core_hpp/ServerCore.hpp"
#include    "../../includes/http_hpp/HttpRequest.hpp"

bool    dir_exists(const std::string &path);
bool    is_writable_dir(const std::string &path);
std::string get_parent_dir(const std::string &path);

std::string get_header_value(const HttpRequest &req, const std::string &name);
size_t  parse_size_vlue(const std::string &val);
size_t get_client_max_body_size(const Server &server);
bool file_exists(const std::string &path);
bool is_writable_file(const std::string &path);
bool is_cgi_request(const Location &loc, const std::string &fullpath);

std::string generate_filename(const std::string &content_type);
bool extract_multipart_file(const std::string &body, const std::string &boundary, size_t &data_start, size_t &data_len);
std::string extract_multipart_filename(const std::string &body);

#endif