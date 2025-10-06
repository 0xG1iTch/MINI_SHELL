NAME = minishell
LIB = libft/libft.a
CFLAGS = -Wall -Werror -Wextra
CC = cc
SRC = main.c free_func.c syntax_error2.c syntax_error.c expanding_func.c expanding_func2.c expanding_func3.c \
exec_list_func.c exec_list_func2.c parsing.c helper_functions.c helper_functions2.c helpers2.c helpers3.c tokenize_input2.c\
signals.c parsing_quotes2.c expanding_func6.c exec_list_func3.c\
helpers.c tokenize_input.c parsing_quotes.c expanding_func4.c expanding_func5.c \
executing/execute.c executing/cd.c executing/echo.c executing/exit.c executing/heredoc.c executing/export_mode.c executing/cd_utils.c \
executing/unset.c executing/pwd.c executing/redirection.c executing/pipe.c executing/export.c executing/env.c executing/libft_helpers1.c\
executing/path.c executing/expanding_heredoc.c executing/get_args.c executing/ft_strtol.c
libs = ./libft/libft.a

READLINE_COMPILE = -I/usr/include
READLINE_LINK = -lreadline

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(libs)
	$(CC) $(OBJ) -o $(NAME) $(LIB) $(READLINE_LINK)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@ $(READLINE_COMPILE)

$(libs):
	@make -C libft

clean:
	@make clean -C libft
	rm -rf $(OBJ)

fclean: clean
	@make fclean -C libft
	rm -rf $(NAME)

re: fclean all
