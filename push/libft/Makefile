NAME = libft.a

CFLAGS = -Wall -Wextra -Werror

RM = rm -f

HEADER = libft.h

CC = cc

SRC = ft_atoi.c ft_itoa.c  ft_putendl_fd.c ft_strlcat.c ft_substr.c\
ft_bzero.c ft_putnbr_fd.c ft_strlcpy.c ft_tolower.c\
ft_calloc.c ft_memchr.c ft_putstr_fd.c ft_strlen.c ft_toupper.c\
ft_isalnum.c ft_memcmp.c ft_split.c ft_strmapi.c\
ft_isalpha.c ft_memcpy.c ft_strchr.c ft_strncmp.c\
ft_isascii.c ft_memmove.c ft_strdup.c ft_strnstr.c\
ft_isdigit.c ft_memset.c ft_striteri.c ft_strrchr.c\
ft_isprint.c ft_putchar_fd.c ft_strjoin.c ft_strtrim.c


OBJ = $(SRC:%.c=%.o)


GREEN = \033[0;32m 
RED = \033[0;31m
PURPLE = \033[0;35m

all : $(NAME)

$(NAME) : $(OBJ) ${HEADER}
	@ar rc $(NAME) $(OBJ)
	@echo "$(PURPLE)TOUT EST MAKE LIBFT"

%.o:%.c $(HEADER)
	@$(CC) $(CFLAGS) -o $@ -c $<

clean :
	@$(RM) $(OBJ)

fclean : clean
	@$(RM) $(NAME)

re : fclean $(NAME)
