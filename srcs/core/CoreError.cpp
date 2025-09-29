# include "../../includes/webserv.hpp"


std::string intTo_String(int n)
{
    if (n == 0)
        return "0";
    std::string str;
    int num = n;
    if (n < 0) {
        num = -n;
    }
    while (num > 0) {
        char digit = '0' + (num % 10);
        str = digit + str;
        num /= 10;
    }
    if (n < 0)
        str = '-' + str;
    return str;
}

CoreError::CoreError(std::string what_failed, int erno_value, std::string error_msg)
{
    mesg = what_failed + " faild: [" + intTo_String(erno_value) + "] "+ error_msg;
}