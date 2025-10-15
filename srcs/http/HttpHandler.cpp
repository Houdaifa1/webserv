#include "../../includes/webserv.hpp"

HttpHandler::HttpHandler(Connection &connection) : connection(connection)
{
     std::cout << connection.client_fd
               << " | Client: " << connection.client_ip << ":" << connection.client_port
               << " | Server: " << connection.server_ip << ":" << connection.server_port << std::endl;

     correct_path();
     check_final_path();
     handle_get();
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

void HttpHandler::correct_path()
{
     std::string raw_path = connection.request.get_requestpath();
     std::string query;

     size_t index = raw_path.find("#");
     raw_path = raw_path.substr(0, index);

     index = raw_path.find("?");
     query = raw_path.substr(index + 1);
     connection.request.set_query(query);

     raw_path = raw_path.substr(0, index);
     if (!decode_path(raw_path))
          std::cout << "error in decoding\n";
     normalize_path(raw_path);
     std::cout << "Normalized path: " << raw_path << "\n";
     if (!find_location(connection.server, raw_path, connection.location))
          std::cout << "Error: no matching location found\n";

     std::string loc_path = connection.location.path;
     normalize_path(loc_path);
     connection.request.set_correct_path(raw_path.substr(loc_path.size()));
}

void HttpHandler::check_final_path()
{
     std::string root = connection.location.root;
     normalize_path(root);
     std::string request_path = connection.request.get_correct_path();
     std::string final_path = root  + "/" + request_path;
     normalize_path(final_path);
     connection.request_full_path = final_path;
     std::cout << "\n\ncorrect path : " << final_path << "\n\n";
}

PathCheck HttpHandler::check_path_exist()
{
     struct stat check;

     if (stat(connection.request_full_path.c_str(), &check) == 0)
     {
          if (S_ISDIR(check.st_mode))
          {
               
               if (opendir(connection.request_full_path.c_str()) == NULL)
                    return Error;
               else 
                    return Directory;

          }
          else if (S_ISREG(check.st_mode))
          {
               if (access(connection.request_full_path.c_str(), R_OK) == 0)
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
    mime_types["htm"]  = "text/html";
    mime_types["css"]  = "text/css";
    mime_types["js"]   = "application/javascript";
    mime_types["json"] = "application/json";
    mime_types["png"]  = "image/png";
    mime_types["jpg"]  = "image/jpeg";
    mime_types["jpeg"] = "image/jpeg";
    mime_types["gif"]  = "image/gif";
    mime_types["svg"]  = "image/svg+xml";
    mime_types["ico"]  = "image/x-icon";
    mime_types["txt"]  = "text/plain";
    mime_types["xml"]  = "application/xml";
    mime_types["pdf"]  = "application/pdf";
    mime_types["zip"]  = "application/zip";
    mime_types["tar"]  = "application/x-tar";
    mime_types["gz"]   = "application/gzip";
    mime_types["mp3"]  = "audio/mpeg";
    mime_types["mp4"]  = "video/mp4";
    mime_types["webm"] = "video/webm";

    std::map<std::string, std::string>::iterator it = mime_types.find(ext);
    if (it != mime_types.end())
        return it->second;
    return "application/octet-stream";
}

void HttpHandler::handle_get()
{
     int status_code;
     PathCheck check = check_path_exist();
     
     if (check == Error)
          std::cout << "403 forbiden \n\n\n";
     if (check == File)
     {
          std::string ext;
          std::string type;

          status_code = 200;
          size_t dot = connection.request_full_path.find_last_of('.');
          if (dot == std::string::npos || dot == connection.request_full_path.length() - 1)
               ext = "";
          ext = connection.request_full_path.substr(dot + 1);
          HttpResponse response(connection, status_code, type, file_size, connection.request_full_path);
          response.sendresponse();
     }
     if (check == Directory)
     {
          // to be done oct 15
     }
     
}

void HttpHandler::handle_post()
{
     std::string correct_path = connection.request.get_correct_path();
     if (!is_method_allowed(connection.location, "POST"))
     {
          std::string resp =
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Allow: POST\r\n"
            "Content-Length: 0\r\n"
            "\r\n";
          send(connection.client_fd, resp.c_str(), resp.size(), 0);
          return;
     }
     std::string    server_root = resolve_upload_path(connection.location);
     std::string    location_path = connection.location.path;
     std::string    relative_path = correct_path;

     if (relative_path.find(location_path) == 0)
          relative_path.erase(0, location_path.length());
     std::string fullpath = make_fullpath(server_root, relative_path);

}

void HttpHandler::handle_delete()
{
}