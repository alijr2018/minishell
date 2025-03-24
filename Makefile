NAME = minishell

SRCS = main.c\
		ft_printf.c \
		ft_putstr.c \

CC = cc
CFLAGS = -Wall -Wextra -Werror
RED = -lreadline
RM = rm -f

OBJS = $(SRCS:.c=.o)

all: $(NAME) 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(RED)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean