#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
# include <stdarg.h>

// #include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int		ft_printf(const char *in, ...);
int		ft_putstr(char *s);
int     ft_putchar(char c);

#endif