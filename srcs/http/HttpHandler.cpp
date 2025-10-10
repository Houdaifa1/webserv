# include "../../includes/webserv.hpp"

HttpHandler::HttpHandler(Connection &connection) : connection(connection)
{
     std::cout << connection.client_fd
              << " | Client: " << connection.client_ip << ":" << connection.client_port
              << " | Server: " << connection.server_ip << ":" << connection.server_port << std::endl;
             
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

     return((v1 * 16) + v2);
}

bool HttpHandler::decode_path(std::string &path)
{
     std::string decoded_path;
     size_t i = 0;
     size_t path_length = path.size();

     char h1, h2;
     int byte_val;

     while(i < path_length)
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
     return  true;
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
          std::cout << "error in decoding\n"; // return false
     
     

}


void HttpHandler::handle_get()
{
   

}

void HttpHandler::handle_post()
{


}

void HttpHandler::handle_delete()
{


}