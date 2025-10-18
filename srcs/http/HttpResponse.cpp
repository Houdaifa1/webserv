#include "../../includes/webserv.hpp"


HttpResponse::HttpResponse(Connection &connection, int status, const std::string type, size_t length, const std::string file)
    :   connection(connection),
        status_code(status),
        content_type(type),
        content_length(length),
        body_file_path(file) {}

HttpResponse::HttpResponse(int status, Connection &connection, const std::string &type, size_t length, const std::string &body)
    : connection(connection),
      status_code(status),
      content_type(type),
      content_length(length),
      body_string(body) {}


std::string get_error(int code)
{
    switch (code)
    {
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 204:
            return "No Content";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 400:
            return "Bad Request";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 413:
            return "Payload Too Large";
        case 500:
            return "Internal Server Error";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        default:
            return "Unknown";
    }
}

std::string HttpResponse::build_headers()
{
    std::string headersStr = "HTTP/1.1 " 
        + intTo_String(status_code) + " " 
        + get_error(status_code) + "\r\n";

    headersStr += "Content-Type: " + content_type + "\r\n";
    headersStr += "Content-Length: " + intTo_String(content_length) + "\r\n";
    headersStr += "\r\n";

    return headersStr;
}


void HttpResponse::sendresponse()
{
    std::string response = build_headers();

    send(connection.client_fd, response.c_str(), response.size(), 0);


    if (!body_string.empty())
    {
        send(connection.client_fd, body_string.c_str(), body_string.size(), 0);
        return;
    }
    FILE *file = fopen(body_file_path.c_str(), "rb");
    if (!file) {
        return;
    }

    char buffer[8192];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(connection.client_fd, buffer, bytesRead, 0);
    }
    fclose(file);
}


// ERROR INDEX

