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

        void correct_path();
        void handle_get();
        bool decode_path(std::string &path);
        void handle_post();
        void handle_delete();
        void check_final_path();
        PathCheck check_path_exist();
        std::string get_type(const std::string &ext);
};



# endif 