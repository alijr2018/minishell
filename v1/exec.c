/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:01 by abrami            #+#    #+#             */
/*   Updated: 2025/05/25 15:06:50 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_return(char *full_path, char *dir, char **paths, char *str)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		full_path = malloc(ft_strlen(paths[i]) + ft_strlen(str) + 2);
		if (!full_path)
		{
			i++;
			continue ;
		}
		strcpy(full_path, paths[i]);
		strcat(full_path, "/");
		strcat(full_path, str);
		if (access(full_path, X_OK) == 0)
			return (ft_strdup(full_path));
		free(full_path);
		i++;
	}
	return (NULL);
}

static char	*ft_check(char *str)
{
	if (access(str, X_OK) == 0)
		return (ft_strdup(str));
	return (NULL);
}

static char	*searchexec(char *str)
{
	char	*path;
	char	*full_path;
	char	*dir;
	char	**paths;
	int		i;

	path = getenv("PATH");
	if (!str || !*str)
		return (NULL);
	if (ft_strchr(str, '/'))
		ft_check(str);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	dir = ft_strtok(path, ":");
	i = 0;
	while (dir)
	{
		paths[i++] = dir;
		dir = ft_strtok(NULL, ":");
	}
	paths[i] = NULL;
	ft_return(full_path, dir, paths, str);
}

void	command_cd(char **alt)
{
	char	*path;

	if (!alt[1])
	{
		path = getenv("HOME");
		if (!path)
			return ;
	}
	else
		path = alt[1];
	if (chdir(path) != 0)
		perror("cd");
}

int handle_options(char **args, int *i, int *no_newline, int *interpret_backslashes)
{
	while (args[*i] && args[*i][0] == '-' && args[*i][1] != '\0')
	{
		if (ft_strcmp(args[*i], "-n") == 0)
			*no_newline = 1;
		else if (ft_strcmp(args[*i], "-e") == 0)
			*interpret_backslashes = 1;
		else if (ft_strcmp(args[*i], "-E") == 0)
			*interpret_backslashes = 0;
		else
			break;
		(*i)++;
	}
	return (*i);
}

char *remove_quotes(char *arg)
{
	int	len;
	
	len = ft_strlen(arg);
	if ((arg[0] == '"' && arg[len - 1] == '"') || (arg[0] == '\'' && arg[len - 1] == '\''))
	{
		arg[len - 1] = '\0';
		arg++;
	}
	if (*arg == '\0')
		return (arg);
	while (*arg && isspace((unsigned char)*arg)) 
		arg++;
	return (arg);
}

char *process_escape_sequences(char *arg, int *interpret_backslashes)
{
	char	*new_arg;
	int		j;
	int		k;

	if (*interpret_backslashes)
	{
		new_arg = malloc(ft_strlen(arg) + 1);
		j = 0;
		k = 0;
		while (arg[k] != '\0')
		{
			if (arg[k] == '\\' && arg[k + 1] != '\0')
			{
				k++;
				if (arg[k] == 'n')
					new_arg[j++] = '\n';
				else if (arg[k] == 't')
					new_arg[j++] = '\t';
				else if (arg[k] == '\\')
					new_arg[j++] = '\\';
				else
					new_arg[j++] = arg[k];
			}
			else
				new_arg[j++] = arg[k];
			k++;
		}
		new_arg[j] = '\0';
	    return (new_arg);
    }
    return (arg);
}

void print_arguments(char **args, int i, int interpret_backslashes)
{
	char	*arg;
	char	*processed_arg;

    while (args[i])
    {
        arg = args[i];
        arg = remove_quotes(arg);
		processed_arg = process_escape_sequences(arg, &interpret_backslashes);
        printf("%s", processed_arg);
        if (args[i + 1])
            printf(" ");
        i++;
    }
}

void ft_echo(char **args)
{
    int	i;
    int	no_newline;
    int	interpret_backslashes;

	i = 1;
	no_newline = 0;
    interpret_backslashes = 0;
    i = handle_options(args, &i, &no_newline, &interpret_backslashes);
    print_arguments(args, i, interpret_backslashes);
    if (!no_newline)
        printf("\n");
}
void	exec(char **alt)
{
	char	*exec_path;

	if (ft_strcmp(*alt, "cd") == 0)
		return (command_cd(alt));
	if (ft_strcmp(*alt, "echo") == 0)
		return (ft_echo(alt));
	if (ft_strcmp(*alt, "$?") == 0)
	{
		printf("0: command not found\n");
		return ;
	}
	exec_path = searchexec(*alt);
	if (!exec_path)
	{
		printf("Command not found: %s\n", *alt);
		exit(127);
	}
	if (execve(exec_path, alt, NULL) == -1)
		perror("Error executing file");
	exit(1);
}
