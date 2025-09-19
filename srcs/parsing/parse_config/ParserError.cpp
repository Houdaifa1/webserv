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

Parsererror::Parsererror(ParserErrorType type, std::string value , std::string &path, int line)
{
    std::string error_location = path + " : Line " + intToString(line) + "\n";

    switch (type)
    {
        case UnknownDirective:
        {
            mesg = "Expected directive name but found \"" + value + "\" in ";
            break;
        }
        case DirectiveNotTerminated:
        {
            mesg = "Directive \"" + value + "\" is not terminated by \";\" in ";
            break;
        }
        case UnexpectedEOF:
        {
            mesg = "Unexpected EOF in ";
            break;
        }
        case ExpectBlockStarter:
        {
            mesg = "Expected \'{\' found \"" + value + "\" in ";
            break;
        }
        case UnexpectedSymbol:
        {
            mesg = "Unexpected Symbol \"" + value + "\" in ";
            break;
        }
        case UnexpectedEOFend:
        {
            mesg = "Unexpected EOF: expected \'}\' in ";
            break;
        }
        case ExpectedPath:
        {
            mesg = "Expected \'Path\' after \"location\" found \"" + value + "\" in ";
            break;
        }
        case NestedBlocks:
        {
            mesg = "Unexpected ‘" + value + "’ directive inside a " + value + " block in ";
            break;
        }
        case TopBolockLow:
        {
            mesg = "Unexpected ‘" + value + "’ directive inside a location block in ";
            break;
        }
    }
    mesg += error_location;
}