# ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "../Libraries.hpp"

class HttpRequest
{
    private :

        std::string                         httpmethod;
        std::string                         requestpath;
        std::string                         httpversion;
        std::map<std::string, std::string>  headers;
        std::string                         body;
    
    public :

        HttpRequest();

        std::string get_httpmethod() const;
        std::string get_requestpath() const;
        std::string get_httpversion() const;
        std::map<std::string, std::string> get_headers() const;
        std::string get_body() const;

        void set_httpmethod(std::string Newhttpmethod);
        void set_requestpath(std::string Newrequestpath);
        void set_httpversion(std::string Newhttpversion);
        void set_headers(std::string key, std::string value);
        void set_body(std::string Newbody);

};

# endif