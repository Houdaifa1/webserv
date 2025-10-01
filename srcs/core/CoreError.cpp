# include "../../includes/webserv.hpp"


CoreError::CoreError(std::string what_failed, int erno_value, std::string error_msg)
{
    mesg = what_failed + " failed: [" + intTo_String(erno_value) + "] "+ error_msg;
}