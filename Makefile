NAME        = webserv

CC          = c++
CFLAGS      = -Wall -Wextra -Werror  -std=c++98

SRC_DIR     = srcs
PARS_DIR    = srcs/parsing
TKN_SRC_DIR = srcs/parsing/parse_tokens
CNF_SRC_DIR = srcs/parsing/parse_config

SRC         = $(SRC_DIR)/main.cpp           \
              $(PARS_DIR)/get_cnf.cpp        \
              $(TKN_SRC_DIR)/Token.cpp        \
              $(TKN_SRC_DIR)/Tokenizer.cpp     \
              $(TKN_SRC_DIR)/TokenizerError.cpp \
              $(CNF_SRC_DIR)/Config.cpp          \
              $(CNF_SRC_DIR)/Parser.cpp           \
              $(CNF_SRC_DIR)/ParserError.cpp       \
              $(CNF_SRC_DIR)/parse_directives.cpp   \
              

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:

fclean:
	rm -rf $(NAME)

re: fclean all
