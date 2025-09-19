# include "../../../includes/webserv.hpp"


void display_directive(Directive &directive)
{
    std::cout << directive.name ;
    for (size_t i = 0; i < directive.args.size(); i++)
    {
        std::cout << "   " << directive.args[i];
    }
    std::cout << std::endl;

}


void display_location(Location &location)
{
    std::cout << "location path :   " << location.path << std::endl;

    for (size_t i = 0; i < location.directives.size(); i++)
    {
        display_directive(location.directives[i]);
    }
}

void Config::display_config()
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        std::cout << "\nDirectives in sever : \n";
        for(size_t j = 0; j < servers[i].directives.size(); j++)
        {
            display_directive(servers[i].directives[j]);
        }
        std::cout << "\n\nLocation in sever : \n";
        std::cout << "Directives in location : \n";
        for(size_t j = 0; j < servers[i].locations.size(); j++)
        {
            display_location(servers[i].locations[j]);
            std::cout << std::endl;
        }
    }


}