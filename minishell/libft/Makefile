CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = libft.a
SRCS = 	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_strlen.c ft_strncmp.c ft_strtrim.c \
		ft_memset.c ft_memcpy.c ft_strlcpy.c ft_strrchr.c ft_atoi.c ft_strjoin.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_itoa.c ft_strdup.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME):$(OBJS)
	ar rc $(NAME) $(OBJS)

%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@ 

bonus: $(OBJS)
	ar rc $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean