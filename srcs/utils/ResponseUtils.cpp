#include "../../includes/webserv.hpp"


void send_created_html(int client_fd, const std::string &path)
{
    std::stringstream ss;
    ss << "<!DOCTYPE html>\n"
       << "<html lang=\"en\">\n"
       << "<head>\n"
       << "<meta charset=\"UTF-8\">\n"
       << "<title>201 Created</title>\n"
       << "<style>\n"
       << "body { font-family: Arial, sans-serif; background: #f9f9f9; color: #333; text-align:center; margin-top:50px; }\n"
       << "h1 { color: #2e7d32; }\n"
       << "a { color: #1e88e5; text-decoration:none; }\n"
       << "</style>\n"
       << "</head>\n"
       << "<body>\n"
       << "<h1>201 Created</h1>\n"
       << "<p>The resource <strong>" << path << "</strong> has been successfully created.</p>\n"
       << "<a href=\"" << path << "\">View File</a>\n"
       << "</body>\n"
       << "</html>";

    std::string body = ss.str();
    std::stringstream header;
    header << "HTTP/1.1 201 Created\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << body.size() << "\r\n"
           << "Connection: close\r\n\r\n";

    std::string response = header.str() + body;
    send(client_fd, response.c_str(), response.size(), 0);
}

void send_delete_confirmation(int client_fd, const std::string &path)
{
    std::stringstream ss;
    ss << "<!DOCTYPE html>\n"
       << "<html lang=\"en\">\n"
       << "<head>\n"
       << "<meta charset=\"UTF-8\">\n"
       << "<title>File Deleted</title>\n"
       << "<style>\n"
       << "body { font-family: Arial, sans-serif; background: #f9f9f9; color: #333; text-align:center; margin-top:50px; }\n"
       << "h1 { color: #d32f2f; }\n"
       << "a { color: #1e88e5; text-decoration:none; }\n"
       << "</style>\n"
       << "</head>\n"
       << "<body>\n"
       << "<h1>File Deleted</h1>\n"
       << "<p>The resource <strong>" << path << "</strong> was successfully deleted.</p>\n"
       << "<a href=\"/\">Return Home</a>\n"
       << "</body>\n"
       << "</html>";

    std::string html = ss.str();

    std::ostringstream header;
    header << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << html.size() << "\r\n"
           << "Connection: close\r\n"
           << "\r\n";

    std::string response = header.str() + html;
    send(client_fd, response.c_str(), response.size(), 0);
}

