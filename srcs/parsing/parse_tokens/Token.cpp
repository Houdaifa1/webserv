# include "../../../includes/webserv.hpp"

Token::Token() : type(UNDEFINED), line(1), col(1) {}


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
        case UNDEFINED:         return "UNDEFINED";
        case IDENTIFIER:        return "IDENTIFIER";
        case STRING:            return "STRING";
        case SYMBOL:            return "SYMBOL";
        case NUMBER:            return "NUMBER";
        case SIZE:              return "SIZE";
        case PATH:              return "PATH";
        case COMMENT:           return "COMMENT";
        case LOCATION_MODIFIER: return "LOCATION_MODIFIER";
    }
    return "UNDEFINED";
}


void Token::display_token()
{
    std::cout << tokenTypeToString(type) << "  :  ";
    std::cout << value << "     ";
    std::cout << line << "  ,  ";
    std::cout << col << "\n";
}

