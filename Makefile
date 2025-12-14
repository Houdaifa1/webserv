NAME        = webserv

CC          = c++
CFLAGS      = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address

SRC_DIR        = srcs
PARS_DIR       = srcs/parsing
TKN_SRC_DIR    = srcs/parsing/parse_tokens
CNF_SRC_DIR    = srcs/parsing/parse_config
CORE_SRC_DIR   = srcs/core
HELP_FUNCS_DIR = srcs/helper_funcs
HTTP_SRC_DIR   = srcs/http
MULTIPLEXING   = srcs/multiplexing
UTILS_SRC      = srcs/utils
CGI_SRC        = srcs/cgi

SRC         = $(SRC_DIR)/main.cpp           \
              $(PARS_DIR)/get_cnf.cpp        \
              $(TKN_SRC_DIR)/Token.cpp        \
              $(TKN_SRC_DIR)/Tokenizer.cpp     \
              $(TKN_SRC_DIR)/TokenizerError.cpp \
              $(CNF_SRC_DIR)/Config.cpp        \
              $(CNF_SRC_DIR)/Parser.cpp         \
              $(CNF_SRC_DIR)/Error_Pages.cpp     \
              $(CNF_SRC_DIR)/ParserError.cpp      \
              $(CNF_SRC_DIR)/parse_directives.cpp  \
              $(CORE_SRC_DIR)/CoreError.cpp \
              $(CORE_SRC_DIR)/ServerCore.cpp \
              $(CORE_SRC_DIR)/StartServer.cpp \
              $(HTTP_SRC_DIR)/HttpRequest.cpp  \
              $(HTTP_SRC_DIR)/ErrorHandler.cpp  \
              $(HTTP_SRC_DIR)/HttpHandler.cpp    \
              $(HTTP_SRC_DIR)/HttpResponse.cpp    \
              $(HELP_FUNCS_DIR)/into_string.cpp    \
              $(HTTP_SRC_DIR)/parse_http_request.cpp   \
              $(HELP_FUNCS_DIR)/remove_white_spaces.cpp \
              $(MULTIPLEXING)/EventLoop.cpp       \
              $(MULTIPLEXING)/EventLoop_utils.cpp  \
              $(MULTIPLEXING)/Connection.cpp        \
              $(UTILS_SRC)/ConfigUtils.cpp           \
              $(UTILS_SRC)/PostUtils.cpp              \
              $(UTILS_SRC)/ResponseUtils.cpp           \
              $(UTILS_SRC)/logs.cpp                     \
              $(UTILS_SRC)/Signals.cpp                   \
              $(CGI_SRC)/cgihandler.cpp                   \
              $(CGI_SRC)/Environment.cpp                   


OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all