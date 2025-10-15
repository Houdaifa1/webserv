#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "../webserv.hpp"

class HttpResponse
{

    private:

        Connection  connection;
        int         status_code;
        std::string content_type;
        size_t      content_length;
        std::string body_file_path;
        
        std::map<std::string, std::string>  extra_headers;

       
    public:

        HttpResponse(Connection &connection, int status, const std::string type, size_t length, const std::string file);
        std::string build_headers();
        void sendresponse();
};

#endif 
