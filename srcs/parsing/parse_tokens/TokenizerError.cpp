# include "../../../includes/webserv.hpp"



TokenizerError::TokenizerError(ErrorType type, int col, int line, char badchar)
{
    std::string position = "[line " + intTo_String(line) + " , col " +
        intTo_String(col) + "] ";
    switch (type)
    {
        case FileNotFound:
            mesg = "File not found\n";
            break;
        case EmptyFile:
            mesg = "Empty File\n";
            break;
        case FileAccessDenied:
            mesg = "File Access Denied\n";
            break;
        case InvalidIdentifierChar:
            mesg += position;
            mesg += "Invalid character '" + std::string(1, badchar) + "' for Identifier\n";
            break;
        case UnclosedQuote:
            mesg += position;
            mesg += "Unclosed Quote need <" + std::string(1, badchar) + ">\n";
            break;
        case InvalidChar:
            mesg += position;
            mesg += "Invalid Chararcter '" + std::string(1, badchar) + "'\n";
            break;
    }
}