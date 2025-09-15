NAME        = webserv

CC          = c++
CFLAGS      = -Wall -Wextra -Werror -w -std=c++98

SRC_DIR     = srcs
CLS_SRC_DIR = includes/classes_code

SRC         = $(SRC_DIR)/main.cpp \
              $(SRC_DIR)/get_config.cpp
              $(CLS_SRC_DIR)/Token.cpp \
              $(CLS_SRC_DIR)/Tokenizer.cpp \
              $(CLS_SRC_DIR)/TokenizerError.cpp

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:

fclean:
	rm -rf $(NAME)

re: fclean all
