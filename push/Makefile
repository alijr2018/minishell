# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 10:51:21 by abrami            #+#    #+#              #
#    Updated: 2025/06/14 11:29:08 by abrami           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishelll

SRCS =	apply_redirections.c \
		exec.c \
		ft_cd.c \
		ft_echo.c \
		ft_env.c \
		ft_exit.c \
		ft_export.c \
		ft_pwd.c \
		ft_unset.c \
		handl_builting.c \
		handl_path.c \
		handl_redirections.c \
		herdoc.c \
		sign_.c \
		token.c \
		utils_fun.c \
		parse.c \
		ft_splited.c \
		minishell.c \
		main.c \
		

CC = cc
# CFLAGS = -Werror -Wextra -Wall
# CF = -g3 -fsanitize=address
RED = -lreadline 
RM = rm -f

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(CF) $(OBJS) -L$(LIBFT_DIR) -lft -o $@ $(RED)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) $(CF) -c $< -o $@

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re