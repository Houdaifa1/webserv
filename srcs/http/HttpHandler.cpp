#include "../../includes/webserv.hpp"

HttpHandler::HttpHandler(Connection &connection) : connection(connection)
{
     std::cout << connection.client_fd
               << " | Client: " << connection.client_ip << ":" << connection.client_port
               << " | Server: " << connection.server_ip << ":" << connection.server_port << std::endl;

     correct_path();
     check_final_path();
     std::string method = connection.request.get_httpmethod();
     if (method == "GET")
          handle_get();
     else if (method == "POST")
          handle_post();
     else if (method == "DELETE")
          handle_delete();
}

bool check_is_hex(char h)
{
     bool check = false;
     if ((h >= '0' && h <= '9'))
          check = true;
     if ((h >= 'a' && h <= 'f'))
          check = true;
     if ((h >= 'A' && h <= 'F'))
          check = true;
     return (check);
}

int get_hex_value(char h1, char h2)
{
     int v1;
     int v2;

     v1 = 0;
     v2 = 0;

     v1 += 0;
     v2 += 0;
     if ((h1 >= '0' && h1 <= '9'))
          v1 = h1 - 48;
     if ((h2 >= '0' && h2 <= '9'))
          v2 = h2 - 48;

     if ((h1 >= 'a' && h1 <= 'f'))
          v1 = h1 - 87;
     if ((h2 >= 'a' && h2 <= 'f'))
          v2 = h2 - 87;

     if ((h1 >= 'A' && h1 <= 'F'))
          v1 = h1 - 55;
     if ((h2 >= 'A' && h2 <= 'F'))
          v2 = h2 - 55;

     return ((v1 * 16) + v2);
}

bool HttpHandler::decode_path(std::string &path)
{
     std::string decoded_path;
     size_t i = 0;
     size_t path_length = path.size();

     char h1, h2;
     int byte_val;

     while (i < path_length)
     {
          if (path[i] != '%')
          {
               decoded_path.push_back(path[i]);
               i++;
               continue;
          }
          else
          {
               i++;
               if ((path_length - i) < 2)
                    return false;
               h1 = path[i];
               h2 = path[i + 1];
               if (!check_is_hex(h1))
                    return false;
               if (!check_is_hex(h2))
                    return false;
               byte_val = get_hex_value(h1, h2);
               std::cout << "byte : " << byte_val << std::endl;
               if (byte_val == 0)
                    return false;
               if (byte_val == 47)
               {
                    decoded_path.push_back('%');
                    decoded_path.push_back('2');
                    decoded_path.push_back('F');
               }
               else
                    decoded_path.push_back(static_cast<char>(byte_val));
               i += 2;
          }
     }
     path = decoded_path;
     return true;
}

std::vector<std::string> split_path(std::string &path)
{
     std::vector<std::string> path_list;
     std::string new_elemnt;
     size_t slash_pos = 0;
     size_t start = 0;
     size_t end = 0;

     while ((slash_pos = path.find('/', start)) != std::string::npos)
     {
          end = slash_pos - start;
          new_elemnt = path.substr(start, end);
          if (!new_elemnt.empty() && new_elemnt != ".")
               path_list.push_back(new_elemnt);
          start = slash_pos + 1;
     }
     if (start < path.size())
     {
          new_elemnt = path.substr(start);
          if (!new_elemnt.empty() && new_elemnt != ".")
               path_list.push_back(new_elemnt);
     }
     return path_list;
}

void normalize_path(std::string &path)
{
     bool is_absolute = false;
     int last_empty;
     if (path[0] == '/')
          is_absolute = true;
     std::vector<std::string> list = split_path(path);
     std::vector<std::string> stack;
     for (size_t i = 0; i < list.size(); i++)
     {
          if (list[i] == "..")
          {
               if (!stack.empty() && !last_empty)
                    stack.pop_back();
               else if (!is_absolute)
               {

                    stack.push_back(list[i]);
                    last_empty = 1;
               }
          }
          else
          {
               stack.push_back(list[i]);
               last_empty = 0;
          }
     }
     path.clear();
     if (is_absolute)
          path.push_back('/');
     for (size_t i = 0; i < stack.size(); i++)
     {
          path = path + stack[i];
          if (i + 1 != stack.size())
               path += "/";
     }
}

bool find_location(const Server &server, const std::string &normalized_path, Location &location)
{
     bool found = false;
     size_t best_match_len = 0;
     std::string loc_path;

     for (size_t i = 0; i < server.locations.size(); i++)
     {
          const Location &loc = server.locations[i];
          loc_path = loc.path;
          normalize_path(loc_path);
          if (normalized_path.compare(0, loc_path.size(), loc_path) != 0)
               continue;
          if (loc_path != "/" &&
              !(normalized_path.size() == loc_path.size() ||
                (normalized_path.size() > loc_path.size() && normalized_path[loc_path.size()] == '/')))
               continue;
          if (loc_path.size() > best_match_len)
          {
               best_match_len = loc_path.size();
               location = loc;
               found = true;
          }
     }
     return found;
}

bool check_allowed_method(const std::string& method, const Location& location)
{
    for (size_t i = 0; i < location.directives.size(); ++i)
    {
        if (location.directives[i].name == "allowed_methods")
        {
            for (size_t j = 0; j < location.directives[i].args.size(); ++j)
            {
                if (location.directives[i].args[j] == method)
                    return true;
            }
            return false;
        }
    }
    return false;
}

void HttpHandler::correct_path()
{
     ErrorHandler error_mesg(connection.location, connection);

     std::string raw_path = connection.request.get_requestpath();
     std::string query;

     size_t index = raw_path.find("#");
     raw_path = raw_path.substr(0, index);

     index = raw_path.find("?");
     if (index != std::string::npos)
          query = raw_path.substr(index + 1);
     connection.request.set_query(query);

     raw_path = raw_path.substr(0, index);
     if (!decode_path(raw_path))
          error_mesg.generate_error_response(404);
     normalize_path(raw_path);
     if (!find_location(connection.server, raw_path, connection.location))
          error_mesg.generate_error_response(404);
     if (!check_allowed_method(connection.request.get_httpmethod(), connection.location))
          error_mesg.generate_error_response(405);
     std::string loc_path = connection.location.path;
     normalize_path(loc_path);
     connection.request.set_correct_path(raw_path.substr(loc_path.size()));
}

void HttpHandler::check_final_path()
{
     std::string root = connection.location.root;
     normalize_path(root);
     std::string request_path = connection.request.get_correct_path();
     std::string final_path = root + "/" + request_path;
     normalize_path(final_path);
     connection.request_full_path = final_path;
     // std::cout << "\n\ncorrect path : " << final_path << "\n\n";
}

PathCheck HttpHandler::check_path_exist(std::string path)
{
     struct stat check;

     DIR *dir;
     if (stat(path.c_str(), &check) == 0)
     {
          if (S_ISDIR(check.st_mode))
          {
               dir = opendir(path.c_str());
               if (dir == NULL)
                    return Error;
               else
               {
                    closedir(dir);
                    return Directory;
               }
          }
          else if (S_ISREG(check.st_mode))
          {
               if (access(path.c_str(), R_OK) == 0)
               {
                    file_size = check.st_size;
                    return File;
               }
               else
                    return Error;
          }
          else
               return Error;
     }
     else
          return NotFound;
}

std::string HttpHandler::get_type(const std::string &ext)
{
     std::map<std::string, std::string> mime_types;
     mime_types["html"] = "text/html";
     mime_types["htm"] = "text/html";
     mime_types["css"] = "text/css";
     mime_types["js"] = "application/javascript";
     mime_types["json"] = "application/json";
     mime_types["png"] = "image/png";
     mime_types["jpg"] = "image/jpeg";
     mime_types["jpeg"] = "image/jpeg";
     mime_types["gif"] = "image/gif";
     mime_types["svg"] = "image/svg+xml";
     mime_types["ico"] = "image/x-icon";
     mime_types["txt"] = "text/plain";
     mime_types["xml"] = "application/xml";
     mime_types["pdf"] = "application/pdf";
     mime_types["zip"] = "application/zip";
     mime_types["tar"] = "application/x-tar";
     mime_types["gz"] = "application/gzip";
     mime_types["mp3"] = "audio/mpeg";
     mime_types["mp4"] = "video/mp4";
     mime_types["webm"] = "video/webm";

     std::map<std::string, std::string>::iterator it = mime_types.find(ext);
     if (it != mime_types.end())
          return it->second;
     return "application/octet-stream";
}

std::string HttpHandler::get_index_file()
{
     std::string index_full_path;
     for (size_t i = 0; i < connection.location.directives.size(); i++)
     {
          if (connection.location.directives[i].name == "index")
          {
               for (size_t j = 0; j < connection.location.directives[i].args.size(); j++)
               {
                    index_full_path = connection.request_full_path +
                                      "/" + connection.location.directives[i].args[j];
                    if (check_path_exist(index_full_path) == File)
                    {
                         return index_full_path;
                    }
               }
          }
     }
     return "not found";
}

std::string HttpHandler::get_html_entries(std::vector<std::string> &entries)
{
     std::string html = "<html><head><title>Index of " + connection.request_full_path + "</title></head><body>";
     html += "<h1>Index of " + connection.request_full_path + "</h1><ul>";

     for (size_t i = 0; i < entries.size(); ++i)
     {
          std::string entry = entries[i];
          html += "<li><a href=\"" + entry + "\">" + entry + "</a></li>";
     }
     html += "</ul></body></html>";
     return (html);
}

void HttpHandler::handle_get()
{
     int status_code;
     PathCheck check = check_path_exist(connection.request_full_path);
     ErrorHandler error_mesg(connection.location, connection);

     if (check == Error)
     {
          error_mesg.generate_error_response(403);
          return;
     }
     if (check == NotFound)
     {
          error_mesg.generate_error_response(404);
          return;
     }
     if (check == Directory)
     {
          std::string index_path = get_index_file();

          if (index_path != "not found")
          {
               struct stat file;
               connection.request_full_path = index_path;
               stat(index_path.c_str(), &file);
               file_size = file.st_size;
               check = File;
          }
          else
          {
               DIR *dir = opendir(connection.request_full_path.c_str());
               struct dirent *entry;
               std::vector<std::string> entries;
               if (connection.location.autoindex == "none" || connection.location.autoindex == "off")
               {
                    error_mesg.generate_error_response(403);
               }
               else
               {
                    status_code = 200;
                    std::string type = "text/html";
                    while ((entry = readdir(dir)) != NULL)
                    {
                         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                              continue;
                         entries.push_back(entry->d_name);
                    }
                    closedir(dir);
                    std::string html = get_html_entries(entries);
                    HttpResponse response(status_code, connection, type, html.size(), html);
                    response.sendresponse();
               }
          }
     }
     if (check == File)
     {
          std::string ext;
          std::string type;

          status_code = 200;
          size_t dot = connection.request_full_path.find_last_of('.');
          if (dot == std::string::npos || dot == connection.request_full_path.length() - 1)
               ext = "";
          ext = connection.request_full_path.substr(dot + 1);
          type = get_type(ext);
          HttpResponse response(connection, status_code, type, file_size, connection.request_full_path);
          response.sendresponse();
     }

}

void HttpHandler::handle_post()
{
     std::string correct_path = connection.request.get_correct_path();
     ErrorHandler error_mesg(connection.location, connection);
     if (!is_method_allowed(connection.location, "POST"))
     {
          error_mesg.generate_error_response(405);
          return;
     }
     std::string  len_str =  get_header_value(connection.request, "Content-Length");
     size_t    client_max_body_size = get_client_max_body_size(connection.server);

     if (len_str.empty() && !connection.request.is_chunked())
     {
          error_mesg.generate_error_response(411);
          return ;
     }
     if (!len_str.empty() && client_max_body_size > 0)
     {
          size_t    len = std::atoi(len_str.c_str());
          if (len > client_max_body_size)
          {
               error_mesg.generate_error_response(413);
               return ;
          }
     }
     if (connection.request.is_chunked())
     {
          error_mesg.generate_error_response(501);
          return;
     }
     std::string server_root = resolve_upload_path(connection.location);
     std::string fullpath = make_fullpath(server_root, correct_path);
     std::string parent_dir = get_parent_dir(fullpath);
     if (correct_path.empty() || !dir_exists(parent_dir))
     {
          error_mesg.generate_error_response(404);
          return;
     }
     if (!is_writable_dir(parent_dir))
     {
          error_mesg.generate_error_response(403);
          return;
     }
     if (file_exists(fullpath) && !is_writable_file(fullpath))
     {
          error_mesg.generate_error_response(403);
          return;
     }
     if (is_cgi_request(connection.location, fullpath))
     {
          std::cout << "CGI request detected for: " << fullpath << "\n\n\n";
          // yakhadad handle CGI here 
          return;
     }
     std::ofstream ofs(fullpath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
     if (!ofs.is_open() || !ofs.good())
     {
          error_mesg.generate_error_response(500);
          return;
     }
     const std::string &body = connection.request.get_body();
     ofs.write(body.c_str(), body.size());
     if (!ofs.good())
     {
          ofs.close();
          error_mesg.generate_error_response(500);
          return;
     }
     ofs.close();
     send_created_html(connection.client_fd, correct_path);
}

void HttpHandler::handle_delete()
{
     std::string    correct_path = connection.request.get_correct_path();
     ErrorHandler   error_mesg(connection.location, connection);
     if (!is_method_allowed(connection.location, "DELETE"))
     {
          std::cout << "enter isnide method not allowed on DELETE" << std::endl;
          error_mesg.generate_error_response(405);
          return;
     }
     std::string    server_root = resolve_upload_path(connection.location);
     std::string    fullpath = make_fullpath(server_root, correct_path);

     struct stat    st;
     if (stat(fullpath.c_str(), &st) != 0)
     {
          error_mesg.generate_error_response(404);
          return;
     }
     if (S_ISDIR(st.st_mode))
     {
          error_mesg.generate_error_response(403);
          return ;
     }

     std::string    parent_dir = get_parent_dir(fullpath);
     if (!is_writable_dir(parent_dir))
     {
          error_mesg.generate_error_response(403);
          return ;
     }

     if (unlink(fullpath.c_str()) != 0)
     {
          error_mesg.generate_error_response(500);
          return ;
     }
     send_delete_confirmation(connection.client_fd, correct_path);
}