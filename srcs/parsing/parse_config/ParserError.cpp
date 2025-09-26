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
        case NoServerFound:
        {
            mesg = "Error: missing required \"server\" block in configuration in " + path  + "\n";
            return ;
        }
        case GlobalDirective:
        {
            mesg = "Error: Directive <" + value + "> is not allowed in global scope; must be inside a server or location block. in ";
            break;
        }
        case InvalidNumberArgs:
        {
            mesg = "Error: directive " + value + " expects exactly 1 argument in #";
            break;
        }
        case InvalidArgument:
        {
            mesg = "Invalid argument for listen directive in ";
            break;
        }
        case InvalidPortNumber:
        {
            mesg = "Invalid port number <" + value + "> for listen directive; port must be between 1 and 65535 inclusive in ";
            break;
        }
        case MissingAddress:
        {
            mesg = "Error: Listen directive missing address before port in ";
            break;
        }
        case InvalidAddress:
        {
            mesg = "Error: invalid address <" + value + "> for listen directive in ";
            break;
        }
        case MissingListen:
        {
            mesg = "Error: missing required \"listen\" directive for server in " + path  + "\n";
            return ;
        }
    }
    mesg += error_location;
}