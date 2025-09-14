# include "../Token.hpp"

Token::Token() : col(1), line(1), type(UNDEFINED) {}


void Token::set_new_token(const std::string &value,
    int col, int line, TokenType type)
{
    this->value = value;
    this->col = col;
    this->line = line;
    this->type = type;
}

const char* tokenTypeToString(TokenType type)
{
    switch(type)
    {
        case UNDEFINED:    return "UNDEFINED";
        case IDENTIFIER:   return "IDENTIFIER";
        case STRING:       return "STRING";
        case SYMBOL:       return "SYMBOL";
        case NUMBER:       return "NUMBER";
        case SIZE:         return "SIZE";
        case PATH:         return "PATH";
        case COMMENT:      return "COMMENT";
        case END_OF_FILE:  return "END_OF_FILE";
        default:           return "UNKNOWN";
    }
}


void Token::display_token()
{
    // std::cout << tokenTypeToString(type) << "  :  ";
    std::cout << value << "\n";
    // std::cout << line << "  ,  ";
    // std::cout << col << "\n";
}

