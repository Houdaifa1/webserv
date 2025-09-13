# include "../TokenizerError.hpp"


TokenizerError::TokenizerError(int col, int line, char badchar)
{
    mesg += "Error: unexpected character ";
    mesg += badchar;
    mesg += " at line " + std::to_string(line) + " , col " + std::to_string(col) + "\n";
}