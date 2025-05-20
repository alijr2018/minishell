# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 10:51:21 by abrami            #+#    #+#              #
#    Updated: 2025/05/20 17:48:12 by abrami           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishelll

SRCS =	minishell.c \
		ft_utils.c   \
		ft_parsing.c  \
		ft_executing.c  \
		# ft_printf.c \
		# ft_putstr.c \
		# ft_split.c \
		# exec.c \

CC = cc
CFLAGS = -Werror -Wextra -Wall
CF = -g -fsanitize=address
RED = -lreadline 
RM = rm -f

OBJS = $(SRCS:.c=.o)

all: $(NAME) 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CF) $^ -o $@ $(RED)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) $(CF) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean