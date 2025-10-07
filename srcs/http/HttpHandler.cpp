# include "../../includes/webserv.hpp"

HttpHandler::HttpHandler(Connection &connection) : connection(connection)
{
     std::cout << connection.client_fd
              << " | Client: " << connection.client_ip << ":" << connection.client_port
              << " | Server: " << connection.server_ip << ":" << connection.server_port << std::endl;
             
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