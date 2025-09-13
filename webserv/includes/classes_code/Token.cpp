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


void Token::display_token()
{
    std::cout << type << "  :  ";
    std::cout << value << "  ,  ";
    std::cout << line << "  ,  ";
    std::cout << col << "\n";
}