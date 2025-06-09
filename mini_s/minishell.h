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
# define INPUT		1	//"<"
# define HEREDOC	2	//"<<"
# define TRUNC		3	//">"
# define APPEND		4	//">>"
# define PIPE		5	//"|"
# define CMD		6	//"|"
# define ARG		7	//"|"
# define ERR_MALLOC	"malloc error\n"
# define ERR_PIPE	"pipe error\n"
# define ERR_FORK	"fork error\n"

# define EXT_MALLOC	1
# define EXT_PIPE	1
# define EXT_FORK	1

# define IS_QUOTE(x) (x == '"' || x == '\'')
#define apath "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"


typedef struct s_cmd
{
	bool			skip_cmd;
	int				in_file;
	int				out_file;
	char			**cmd_param;
	struct s_cmd	*previous;
	struct s_cmd	*next;
}				t_cmd;


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
	struct s_token	*previous;
	struct s_token	*next;
}				t_token;

typedef struct s_command
{
	char	**args;
	t_list	*env;
	t_token	*token;
	t_cmd	*commands;
	char	*input_file;
	char	*output_file;
	char	*heredoc_delimiter;
	int		append_output;
	int		code_exit;
	bool	sq;
	int		pip[2];
}				t_command;

typedef struct s_stack
{
	t_command					**args;
	size_t				index;
	struct s_stack		*next;
}					t_stack;

int		ft_strcmp(char *src, char *dest);
int ft_strncmp(char *srt, char *str, int n);
// void	ft_executing(char **alt);
// void	ft_executing(char **alt, char **env);
void ft_executing(t_command *cmd, char **env);
char	*ft_strchr(const char *s, int c);
char	**ft_split(const char *s, char c);
size_t 	ft_strlen(const char *str);
char	*ft_strdup(const char *s1);
char	*ft_strtok(char *str, const char *delim);
void	ft_sigaction();
char	*ft_strdup(const char *s1);
int add_to_list(t_list **list, char *str);
char	*ft_strjoin(char const *s1, char const *s2);
// void    ft_execute(char *alt, char **env);

int add_to_list_cmd(t_cmd **list, int infile, int outfile, char **cmd_param);
//parsing
char	**ft_splited(const char *s);
// void	ft_lstclear(t_stack **lst);
void free_char_array(char **arr);
int		parses(t_command *cmd, char *input);
int cd_builtin(char **args);
int echo_builtin(char **args);
void	ft_putnstr(const char *s, int n);

//just for tests


void	print_token(t_token *token);

#endif
