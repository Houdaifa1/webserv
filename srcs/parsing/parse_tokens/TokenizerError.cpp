# include "../../../includes/webserv.hpp"


std::string TokenizerError::intToString(int n)
{
    if (n == 0) return "0";
    std::string str;
    while (n > 0) {
        char digit = '0' + (n % 10);
        str = digit + str; // prepend the digit
        n /= 10;
    }
    return str;
}

TokenizerError::TokenizerError(ErrorType type, int col, int line, char badchar)
{
    std::string position = "[line " + intToString(line) + " , col " +
        intToString(col) + "] ";
    switch (type)
    {
        case FileNotFound:
            mesg = "File not found\n";
            break;
        case EmptyFile:
            mesg = "Empty File\n";
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