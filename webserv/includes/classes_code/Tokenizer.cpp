# include "../Tokenizer.hpp"

Tokenizer::Tokenizer(const std::string &file_name) : file_name(file_name), 
    index(0), col(1), line(1), done(0) {}

int Tokenizer::get_config_in_str()
{
    std::ifstream   file(file_name.c_str());
    if (!file.is_open())
        return (1);
    std::string     line;
    while (std::getline(file, line))
    {
        config += line;
        config += '\n';
    }
    return(0);
}

int Tokenizer::is_only_digits(const std::string &value)
{
    char x;

    for (int i = 0; i < value.size(); i++)
    {
        x = value[i];
        if (!std::isdigit(x))
            return 0;
        
    }
    return (1);
}

int Tokenizer::is_size_digit(const std::string &value, const std::string  &allowed)
{
    char x;

    int i = 0;
    int check = 0;
    while (i < value.size() - 1)
    {
        x = value[i];
        if (!std::isdigit(x))
            return 0;
        i++;
    }
    x = value[i];
    for (int i = 0; i < allowed.size(); i++)
    { 
        if(x == allowed[i])
        {
           
            check = 1;
        }
    }
    return (check);
}

int Tokenizer::not_end_value()
{
    if ((c >= 9 && c <= 13) || c == ' ' || c == '#'
            || c == '\n' || c == '{' || c == '}' || c == ';')
        return 0;
    return 1;
}

int Tokenizer::valid_identifier_char()
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        return 1;
    return 0;
}

void Tokenizer::skip_void()
{
    while ((c >= 9 && c <= 13) || c == ' ' || c == '\n')
    {
        done = 1;
        if (c == '\n')
        {
            col = 0;
            line++;
        }
        index++;
        col++;
        if (index < config.size())
            c = config[index];
        else
            break; 

    }
}

void Tokenizer::is_comment()
{
    if (c == '#')
    {
        done = 1;
        std::string value;
        Token new_token;
        for ( ; index < config.size(); index++)
        {
            c = config[index];
            value += c;
            if (c == '\n')
            {
                line++;
                break ;
            }
        }
        new_token.set_new_token(value, col, line, COMMENT);
        tokens.push_back(new_token);
        col = 1;
        c = config[index];
    }
}

void Tokenizer::is_symbol()
{
    if (c == '{' || c == '}' || c == ';')
    {
        Token       new_token;
        std::string value;

        value += c;
        new_token.set_new_token(value, col, line, SYMBOL);
        tokens.push_back(new_token);
        index++;
        col++;
        c = config[index];
        done = 1;
    }
}

void    Tokenizer::is_digit()
{
    if (c >= '0' && c <= '9')
    {
        int         start_col = col;
        std::string value;
        Token       new_token;
        while(not_end_value())
        {
            value += c;
            index++;
            col++;
            c = config[index];
        }
        if (is_only_digits(value))
            new_token.set_new_token(value, start_col, line, NUMBER);
        else if(is_size_digit(value, "KMG"))
            new_token.set_new_token(value, start_col, line, SIZE);
        else
            new_token.set_new_token(value, start_col, line, STRING);
        tokens.push_back(new_token);
        done = 1;
    }
}

void Tokenizer::is_path()
{
    if (c == '/' || c == '.')
    {
        int         start_col = col;
        std::string value;
        Token       new_token;
        while (not_end_value())
        {
            value += c;
            col++;
            index++;
            c = config[index];
        }
        new_token.set_new_token(value, start_col, line, PATH);
        tokens.push_back(new_token);
        done = 1;
    }
}

void Tokenizer::is_identifier()
{
    if (valid_identifier_char())
    {
        int start_col = col;
        std::string value;
        Token       new_token;
        while(not_end_value())
        {
            if(!valid_identifier_char() && c != '-')
            {
                throw TokenizerError(col, line, c);
            }
            value += c;
            col++;
            index++;
            c = config[index];
        }
        new_token.set_new_token(value, start_col, line, IDENTIFIER);
        tokens.push_back(new_token);
        done = 1;
    }
}

int Tokenizer::tokenize_str()
{
    if (config.empty())
    {
        std::cout << "Config file empty" << std::endl ;
        return 1;
    }
    while(index < config.size())
    {
        done = 0;
        c = config[index];
        skip_void();
        is_comment();
        is_symbol();
        is_digit();
        is_path();
        is_identifier();
        if (done == 0)
        {
            throw TokenizerError(col, line, c);
        }
    }
    return 0;
}

int Tokenizer::Tokenizeall()
{
    try 
    {
        get_config_in_str();
        tokenize_str();
    }
    catch(TokenizerError &error)
    {
        std::cerr << error.mesg;
    }
    return 0;
}


void    Tokenizer::display_config()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        tokens[i].display_token();
    }
}