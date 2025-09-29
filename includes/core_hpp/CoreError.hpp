# ifndef COREERROR_HPP
# define COREERROR_HPP

# include "../Libraries.hpp"


class CoreError
{
    public :
    
        std::string mesg;

        CoreError(std::string what_failed, int erno_value, std::string error_msg);


};


# endif