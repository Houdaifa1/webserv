# ifndef HTTPHANDLER_HPP
# define HTTPHANDLER_HPP

# include "../webserv.hpp"
enum PathCheck
{
    Error,
    Directory,
    File,
    NotFound,

};


class HttpHandler
{
    private :

        Connection &connection;
        size_t  file_size;


    public :

        HttpHandler(Connection &connection);

        bool correct_path();
        void handle_get();
        bool decode_path(std::string &path);
        void handle_post();
        void handle_delete();
        void check_final_path();
        PathCheck check_path_exist(std::string path);
        std::string get_type(const std::string &ext);
        std::string get_index_file();
        std::string get_html_entries(std::vector<std::string> &entries);
};



# endif 