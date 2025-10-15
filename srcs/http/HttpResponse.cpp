#include "../../includes/webserv.hpp"


HttpResponse::HttpResponse(Connection &connection, int status, const std::string type, size_t length, const std::string file)
    :   connection(connection),
        status_code(status),
        content_type(type),
        content_length(length),
        body_file_path(file) {}



std::string HttpResponse::build_headers()
{
    std::string headersStr = "HTTP/1.1 " + intTo_String(status_code) + " ok\r\n";
    headersStr += "Content-Type: " + content_type + "\r\n";
    headersStr += "Content-Length: " + intTo_String(content_length) + "\r\n";
    headersStr += "\r\n";
    return headersStr;
}



void HttpResponse::sendresponse()
{
    std::string response = build_headers();

    send(connection.client_fd, response.c_str(), response.size(), 0);


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