#include "../../includes/webserv.hpp"


ErrorHandler::ErrorHandler(Location &location, Connection &connection) : 
        location (location),
        connection (connection) {}



std::string build_error_body(int code, const std::string& server = "MyWebServer/1.0")
{
    std::map<int, std::string> error_mesg;
    error_mesg[400] = "Bad Request";
    error_mesg[201] = "Created";
    error_mesg[401] = "Unauthorized";
    error_mesg[403] = "Forbidden";
    error_mesg[404] = "Not Found";
    error_mesg[405] = "Method Not Allowed";
    error_mesg[408] = "Request Timeout";
    error_mesg[411] = "Length Required";
    error_mesg[413] = "Payload Too Large";
    error_mesg[414] = "URI Too Long";
    error_mesg[500] = "Internal Server Error";
    error_mesg[501] = "Not Implemented";
    error_mesg[502] = "Bad Gateway";
    error_mesg[503] = "Service Unavailable";
    error_mesg[504] = "Gateway Timeout";


    std::string reason = "Unknown Error";
    std::map<int, std::string>::const_iterator it = error_mesg.find(code);
    if (it != error_mesg.end())
        reason = it->second;

    std::string html =
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head><meta charset=\"UTF-8\"><title>" +
        intTo_String(code) + " " + reason +
        "</title></head>\n"
        "<body style=\"font-family:sans-serif;text-align:center;margin-top:10%;background:#f8f8f8;\">\n"
        "<h1 style=\"color:#c00;font-size:3em;\">" + intTo_String(code) + " " + reason + "</h1>\n"
        "<p style=\"color:#555;font-size:1.2em;\">The server encountered an error and could not complete your request.</p>\n"
        "<hr style=\"border:0;border-top:1px solid #ccc;width:60%;\">\n"
        "<small style=\"color:#888;\">" + server + "</small>\n"
        "</body></html>\n";
    return html;
}

void ErrorHandler::generate_error_response(int error_code)
{
    std::map<std::string, std::string>::iterator current;

    std::string type = "text/html";
    current = location.errors.find(intTo_String(error_code));
    if (current != location.errors.end())
    {
        std::string path = "." + current->second;
        struct stat check;
        if (stat(path.c_str(), &check) == 0)
        {
            if (S_ISREG(check.st_mode) && (access(path.c_str(), R_OK) == 0))
            {   
                HttpResponse resp(connection, error_code, type, check.st_size , path);
                resp.sendresponse();
                return ;
            }
        }
    }
    std::string html = build_error_body(error_code);
    HttpResponse resp(error_code, connection, type, html.size(), html);
    resp.sendresponse();
}