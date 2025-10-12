# include "../../includes/webserv.hpp"


HttpRequest::HttpRequest(){}

std::string HttpRequest::get_httpmethod() const {return httpmethod;}
std::string HttpRequest::get_requestpath() const {return requestpath;}
std::string HttpRequest::get_httpversion() const {return httpversion;}
std::string HttpRequest::get_body() const {return body;}
std::map<std::string, std::string> HttpRequest::get_headers() const {return headers;}
std::string HttpRequest::get_correct_path() const {return correct_path;}

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

void HttpRequest::set_correct_path(std::string Newcorrect_path)
    {correct_path = Newcorrect_path;}
