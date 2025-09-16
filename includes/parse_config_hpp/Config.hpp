# ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Server.hpp" 

class Config
{
    private :

        std::vector<Server>     servers;
        std::vector<Directive>  globals;
    
    public :
       // Config();


};


# endif