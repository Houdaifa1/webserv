# ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "../Libraries.hpp"

class HttpRequest
{
    private :

        std::string                         httpmethod;
        std::string                         requestpath;
        std::string                         httpversion;
        std::string                         query;
        std::string                         correct_path;
        std::string                         host_domain;
        std::string                         host_port;
        std::map<std::string, std::string>  headers;
        std::string                         body;
    
    public :

        HttpRequest();

        std::string get_httpmethod() const;
        std::string get_requestpath() const;
        std::string get_httpversion() const;
        std::map<std::string, std::string> get_headers() const;
        std::string get_body() const;
        std::string get_correct_path() const;
        std::string get_query() const;
        std::string get_host_domain() const;
        std::string get_host_port() const;

        void set_httpmethod(std::string Newhttpmethod);
        void set_requestpath(std::string Newrequestpath);
        void set_httpversion(std::string Newhttpversion);
        void set_headers(std::string key, std::string value);
        void set_body(std::string Newbody);
        void set_query(std::string Newquery);
        void set_correct_path(std::string Newcorrect_path);
        bool is_chunked() const;
        void set_host_domain(std::string Newhost_domain);
        void set_host_port(std::string Newhost_port);

};

# endif