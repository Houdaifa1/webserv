# ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Server.hpp" 

class Config
{
    public :

        std::vector<Server>                         servers;
        std::vector<Directive>                      globals;
        std::vector<std::pair<std::string, int>>    pairs;
    
        void display_config();
};


# endif