#include "../../../includes/webserv.hpp"

Parsererror::Parsererror(ParserErrorType type, std::string value, std::string &path, int line)
{
    std::string error_location = path + " : Line " + intTo_String(line) + "\n";

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
        mesg = "Error: missing required \"server\" block in configuration in " + path + "\n";
        return;
    }
    case GlobalDirective:
    {
        mesg = "Error: Directive <" + value + "> is not allowed in global scope; must be inside a server or location block. in ";
        break;
    }
    case InvalidNumberArgs:
    {
        mesg = "Error: directive " + value + " expects exactly 1 argument in ";
        break;
    }
    case ErrorCode:
    {
        mesg = "Error: directive " + value + " error code must be a number in ";
        break;
    }
    case InvalidNumberArgs2:
    {
        mesg = "Error: directive " + value + " expects at least 2 arguments in ";
        break;
    }
    case InvalidNumberArgsCgi:
    {
        mesg = "Error: directive " + value + " expects at least 1 arguments in ";
        break;
    }
    case OnOffexpected:
    {
        mesg = "Error: directive " + value + " expects on or off as argument  in ";
        break;
    }
    case DuplicateRoot:
    {
        mesg = "Error: block " + value + " duplicate root directive in ";
        break;
    }
    case DuplicateAutoindex:
    {
        mesg = "Error: block " + value + " duplicate Autoindex directive in ";
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
    case NotPath:
    {
        mesg = "Invalid path <" + value + "> for erro_page directive in ";
        break;
    }
    case MissingAddress:
    {
        mesg = "Error: Listen directive missing ip address before port in ";
        break;
    }
    case InvalidAddress:
    {
        mesg = "Error: invalid ip address <" + value + "> for listen directive in ";
        break;
    }
    case MissingListen:
    {
        mesg = "Error: missing required \"listen\" directive for server in " + path + "\n";
        return;
    }
    case MissingRoot:
    {
        mesg = "Error: missing required \"root\" directive for server in " + path + "\n";
        return;
    }
    case InvalidMethodArgs:
    {
        mesg = "Error: directive " + value + " expects GET/POST/DELETE as argument  in ";
        break;
    }
    case DuplicateAllowedMethods:
    {
        mesg = "Error: block " + value + " duplicate allowed_methods directive in ";
        break;
    }
    case DuplicateUploadStore:
    {
        mesg = "Error: block " + value + " duplicate upload_store directive in ";
        break;
    }
    case DuplicateIndex:
    {
        mesg = "Error: block " + value + " duplicate index directive in ";
        break;
    }
    case DuplicateClientMaxBodySize:
    {
        mesg = "Error: block " + value + " duplicate client_max_body_size directive in ";
        break;
    }
    case DuplicateCgiPath:
    {
        mesg = "Error: block " + value + " duplicate cgi_path directive in ";
        break;
    }
    }
    mesg += error_location;
}