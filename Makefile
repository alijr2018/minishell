# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 10:51:21 by abrami            #+#    #+#              #
#    Updated: 2025/05/25 15:11:16 by abrami           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

SRCS =	minishell.c\
		ft_split.c \
		exec.c \
		ft_utils.c \

CC = cc
# CFLAGS =  -g3 -fsanitize=address
RED = -lreadline 
RM = rm -f

OBJS = $(SRCS:.c=.o)

all: $(NAME) 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ $(RED) -o $@ 

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean
