# include "../../../includes/webserv.hpp"


std::string intToString(int n)
{
    if (n == 0)     
        return "0";
    std::string str;
    while (n > 0)
    {
        char digit = '0' + (n % 10);
        str = digit + str;
        n /= 10;
    }
    return str;
}

Parsererror::Parsererror(ParserErrorType type, std::string value, std::string &path, int line)
{
    std::string error_location = path + ":" + intToString(line) + "\n";

    switch (type)
    {
        case UnknownDirective:
        {
            mesg = " Expected directive name but found \"" + value + "\" in ";
            break;
        }
        case DirectiveNotTerminated:
        {
            mesg = " Directive \"" + value + "\" is not terminated by \";\""
                + " in ";
            break;
        }
    }
    mesg += error_location;
}