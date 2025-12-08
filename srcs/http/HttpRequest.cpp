# include "../../includes/webserv.hpp"


HttpRequest::HttpRequest(){}

std::string HttpRequest::get_httpmethod() const {return httpmethod;}
std::string HttpRequest::get_requestpath() const {return requestpath;}
std::string HttpRequest::get_httpversion() const {return httpversion;}
std::string HttpRequest::get_body() const {return body;}
std::map<std::string, std::string> HttpRequest::get_headers() const {return headers;}
std::string HttpRequest::get_correct_path() const {return correct_path;}
std::string HttpRequest::get_query() const {return query;}
std::string HttpRequest::get_host_domain() const {return host_domain;}
std::string HttpRequest::get_host_port() const {return host_port;}

void HttpRequest::set_httpmethod(std::string Newhttpmethod)
    {httpmethod = Newhttpmethod;}

void HttpRequest::set_requestpath(std::string Newrequestpath)
    {requestpath = Newrequestpath;}

void HttpRequest::set_httpversion(std::string Newhttpversion)
    {httpversion = Newhttpversion;}

void HttpRequest::set_headers(std::string key, std::string value)
    {headers[key] = value;}

void HttpRequest::set_body(std::string Newbody)
    {body = Newbody;}

void HttpRequest::set_query(std::string Newquery)
    {query = Newquery;}

void HttpRequest::set_host_domain(std::string Newhost_domain)
    {host_domain = Newhost_domain;}

void HttpRequest::set_host_port(std::string Newhost_port)
    {host_port = Newhost_port;}

void HttpRequest::set_correct_path(std::string Newcorrect_path)
    {correct_path = Newcorrect_path;}

bool HttpRequest::is_chunked() const
{
    std::map<std::string, std::string>::const_iterator it = headers.find("Transfer-Encoding");
    if (it != headers.end())
    {
        std::string value = it->second;
        for (size_t i = 0; i < value.size(); ++i)
            value[i] = std::tolower(value[i]);
        if (value.find("chunked") != std::string::npos)
            return true;
    }
    return false;
}
