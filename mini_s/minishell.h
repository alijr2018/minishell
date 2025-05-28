/* ************************************************************************** */
/*	                                                                          */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:10 by abrami            #+#    #+#             */
/*   Updated: 2025/03/25 11:14:47 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdarg.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
#define MAX_ARGS 1024
# define PIPE		5	//"|"

#define apath "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

typedef struct s_list
{
	char	*str;
	struct	s_list	*previous;
	struct	s_list	*next_one;
}						t_list;

typedef struct s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

typedef struct s_command
{
	char	**args;
	t_list	*env;
	t_token	*token;
	char	*input_file;
	char	*output_file;
	char	*heredoc_delimiter;
	int		append_output;
	int		code_exit;
}				t_command;


int		ft_strcmp(char *src, char *dest);
// void	ft_executing(char **alt);
void	ft_executing(char **alt, char **env);
char	*ft_strchr(const char *s, int c);
char	**ft_split(const char *s, char c);
size_t 	ft_strlen(const char *str);
char	*ft_strdup(const char *s1);
char	*ft_strtok(char *str, const char *delim);
void	ft_sigaction();
char	*ft_strdup(const char *s1);
int add_to_list(t_list **list, char *str);
char	*ft_strjoin(char const *s1, char const *s2);
void    ft_execute(char **alt, char **env);
bool    parse(t_command *cmd, char  *input);
//just for tests


void	print_token(t_token *token);

#endif
