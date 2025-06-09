# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 10:51:21 by abrami            #+#    #+#              #
#    Updated: 2025/06/09 04:20:07 by abrami           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishelll

SRCS =	minishell.c \
		ft_utils.c   \
		ft_parsing.c  \
		ft_parse_1.c  \
		ft_executing.c  \
		ft_split.c \
		ft_splited.c \
		ft_ctrl.c \
		ft_exit.c \
		ft_strjoin.c \
		ft_built_in.c \
		ft_test.c \
		

CC = cc
CFLAGS = -Werror -Wextra -Wall
CF = -g3 -fsanitize=address
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