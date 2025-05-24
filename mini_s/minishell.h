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
# include <stdarg.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
#define MAX_ARGS 1024

#define apath "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

typedef struct s_command {
	char	**args;
	char	*input_file;
	char	*output_file;
	char	*heredoc_delimiter;
	int		append_output;
}	t_command;

int		ft_strcmp(char *src, char *dest);
// void	ft_executing(char **alt);
void	ft_executing(char **alt, char **env);
char	*ft_strchr(const char *s, int c);
char	**ft_split(const char *s, char c);
size_t 	ft_strlen(const char *str);
char	*ft_strdup(const char *s1);
char	*ft_strtok(char *str, const char *delim);
void	ft_sigaction();

#endif
