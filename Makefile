FILES	= init.c \
		main.c \
		timestamp.c

SRCS	= $(addprefix srcs/, $(FILES))

OBJS	= $(SRCS:.c=.o)

CC		= cc

FLAGS	= -Wall -Wextra -Werror

NAME	= philo

all:	$(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY: re fclean clean all