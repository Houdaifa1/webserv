# include "../../../includes/webserv.hpp"

Tokenizer::Tokenizer(const std::string &file_name) : file_name(file_name),
                                                     index(0), col(1), line(1) {}


std::vector<Token>  Tokenizer::get_tokens()
{
        return tokens;
}
void Tokenizer::get_config_in_str()
{
    std::ifstream file(file_name.c_str());
    if (!file.is_open())
        throw TokenizerError(FileNotFound, col, line);
    std::string line;
    while (std::getline(file, line))
    {
        config += line;
        config += '\n';
    }
}

int Tokenizer::is_only_digits(const std::string &value)
{
    unsigned char x;

    for (int i = 0; i < value.size(); i++)
    {
        x = value[i];
        if (!std::isdigit(x))
            return 0;
    }
    return (1);
}

int Tokenizer::is_size_digit(const std::string &value, const std::string &allowed)
{
    unsigned char x;

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
        if (x == allowed[i])
        {

            check = 1;
        }
    }
    return (check);
}

int Tokenizer::not_end_value()
{
    if ((c >= 9 && c <= 13) || c == ' ' || c == '#' || c == '\n' || c == '{' || c == '}' || c == ';' || c == '\'' || c == '\"')
        return 0;
    return 1;
}

int Tokenizer::valid_identifier_char()
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') 
    || c == '_' || c == '.')
        return 1;
    return 0;
}
bool    Tokenizer::is_location_modifier(std::string &value)
{
    bool loc_mod = false;
    if (c == '~' || c == '=')
    {
        loc_mod = true;
        value += c;
        index++;
        col++;
        if (index < config.size())
            c = config[index];
        else
            return loc_mod;
        if (c == '*')
        {
            value += c;
            index++;
            col++;
        }
    }
    return loc_mod;
}

bool Tokenizer::consume_whitespaces()
{
    bool skipped = false;
    while ((c >= 9 && c <= 13) || c == ' ' || c == '\n')
    {
        skipped = true;
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
    return skipped;
}

bool Tokenizer::consume_comment()
{
    bool comment = false;
    if (c == '#')
    {
        comment = true;
        std::string value;
        Token new_token;
        int start_col = col;
        int start_line = line;
        for (; index < config.size(); index++)
        {
            c = config[index];
            if (c == '\n')
            {
                line++;
                index++;
                 
                break;
            }
            value += c;
        }
        new_token.set_new_token(value, start_col, start_line, COMMENT);
        tokens.push_back(new_token);
        col = 1;
    }
    return comment;
}

bool Tokenizer::consume_symbol()
{
    std::string value;
    Token new_token;

    if (c == '{' || c == '}' || c == ';')
    {
        
        value += c;
        new_token.set_new_token(value, col, line, SYMBOL);
        tokens.push_back(new_token);
        index++;
        col++;
        return true;
    }
    else if (is_location_modifier(value))
    {
        new_token.set_new_token(value, col, line, LOCATION_MODIFIER);
        tokens.push_back(new_token);
        return true;
    }
    else
        return false;
}

bool Tokenizer::consume_digit()
{
    if (c >= '0' && c <= '9')
    {
        int start_col = col;
        std::string value;
        Token new_token;
        while (not_end_value())
        {
            value += c;
            index++;
            col++;
            if (index < config.size())
                c = config[index];
            else
                break;
        }
        if (is_only_digits(value))
            new_token.set_new_token(value, start_col, line, NUMBER);
        else if (is_size_digit(value, "KMG"))
            new_token.set_new_token(value, start_col, line, SIZE);
        else
            new_token.set_new_token(value, start_col, line, STRING);
        tokens.push_back(new_token);
        return true;
    }
    else
        return false;
}

bool Tokenizer::consume_path()
{
    if (c == '/' || c == '.')
    {
        int start_col = col;
        std::string value;
        Token new_token;
        while (not_end_value())
        {
            value += c;
            col++;
            index++;
            if (index < config.size())
                c = config[index];
            else
                break;
        }
        new_token.set_new_token(value, start_col, line, PATH);
        tokens.push_back(new_token);
        return true;
    }
    else
        return false;
}

bool Tokenizer::consume_identifier()
{
    if (valid_identifier_char())
    {
        int start_col = col;
        std::string value;
        Token new_token;
        while (not_end_value())
        {
            if (!valid_identifier_char() && c != '-')
                throw TokenizerError(InvalidIdentifierChar, col, line, c);
            value += c;
            col++;
            index++;
            if (index < config.size())
                c = config[index];
            else
                break;
        }
        new_token.set_new_token(value, start_col, line, IDENTIFIER);
        tokens.push_back(new_token);
        return true;
    }
    else
        return false;
}

bool Tokenizer::consume_quotes()
{
    bool is_quote = false;
    int start_line = line;
    int start_col;
    std::string value;
    while (c == '\'' || c == '\"')
    {
        is_quote = true;
        if (c == '\'' || c == '\"')
        {
            char quote = c;
            
            index++;
            col++;
            start_col = col;
            if (index < config.size())
                c = config[index];
            else
                throw TokenizerError(UnclosedQuote, col, line, quote);
            
            while (index < config.size())
            {
                if (c == '\\')
                {
                    value += c;
                    index++;
                    col++;
                    if (index >= config.size())
                        break;
                    c = config[index];
                    value += c;
                    index++;
                    col++;
                    if (index < config.size())
                        c = config[index];
                    else
                        break;
                }
                else if (c != quote)
                {
                    if (c == '\n')
                    {
                        line++;
                        col = 0;
                    }
                    value += c;
                    index++;
                    col++;
                    if (index < config.size())
                        c = config[index];
                    else
                        break;
                }
                else
                {
                    index++;
                    col++;
                    break;
                }
            }
            if (c != quote)
                throw TokenizerError(UnclosedQuote, col, line, quote);
            if (index < config.size())
                c = config[index];
        }
    }
    if (is_quote)
    {
        Token new_token;
        new_token.set_new_token(value, start_col, start_line, STRING);
        tokens.push_back(new_token);
    }
    return is_quote;
}

void Tokenizer::tokenize_str()
{
    if (config.empty())
       throw TokenizerError(EmptyFile, col , line);
    while (index < config.size())
    {
        c = config[index];
        if (consume_whitespaces())
            continue;
        if (consume_comment())
            continue;
        if (consume_symbol())
            continue;
        if (consume_digit())
            continue;
        if (consume_path())
            continue;
        if (consume_identifier())
            continue;
        if (consume_quotes())
            continue;
        throw TokenizerError(InvalidChar, col, line, c);
    }
}

int Tokenizer::Tokenizeall()
{
    try
    {
        get_config_in_str();
        tokenize_str();
    }
    catch (TokenizerError &error)
    {
        std::cerr << error.mesg;
        return 1;
    }
    return 0;
}

void Tokenizer::display_config()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        tokens[i].display_token();
    }
}