/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:01 by abrami            #+#    #+#             */
/*   Updated: 2025/06/08 15:44:48 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//this is the file that execute the command not the ones that should create

static char	*searchexec(char *str, char **env)
{
    char    *path;
    char    *full_path, *dir, **paths;
    int     i;

    // path = getenv("PATH");
    if (!env)
        path = apath;
    for (int i = 0; env[i] != NULL; i++) {
        if (strncmp(env[i], "PATH=", 5) == 0) {
            path = env[i] + 5;  // Skip "PATH="
            break;
        }
    }
    if(!str || !*str)
    return (NULL);
    if (ft_strchr(str, '/'))
    {
        // search failure for access 
        if (access(str, X_OK) == 0 && access(str, F_OK) == 0)
            return (ft_strdup(str));
        return (NULL);
    }
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
    i = 0;
    while(paths[i])
    {
        full_path = malloc(strlen(paths[i]) + strlen(str) + 2);
        if (!full_path)
        {
            i++;
            continue;
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

// void    ft_execute(char **alt, char **env)
static void    ft_execute(char *alt, char **env)
{
    char    *exec_path;
    char    **argv;

    exec_path = searchexec(alt, env);
    if (!exec_path)
    {
        //add free
        printf("Command not found: %s\n", alt);
        exit(127);
    }
    // char *argv[] = {alt, NULL};
    argv = malloc(sizeof(char *) * 2);
	if (!argv)
	{
		perror("malloc");
		free(exec_path);
		exit(1);
	}
	argv[0] = alt;
	argv[1] = NULL;
    if (execve(exec_path, argv, env) == -1)
    perror("Error executing file");
    exit(1);
}

static int ft_check(char **args)
{
    if (!args || !args[0])
        return 1;

    if (strcmp(args[0], "cd") == 0)
    {
        cd_builtin(&args[1]);
        return 0;
    }
    else if (strcmp(args[0], "echo") == 0)
    {
        echo_builtin(&args[1]);
        return 0;
    }

    return 1; // Not a built-in
}


// add part before here to check for builtin and env etc..
void ft_executing(t_command *cmd, char **env)
{
    int i = 0;
    pid_t pid;

    if (!cmd || !cmd->args)
        return;
    else if (!ft_check(cmd->args))
        return;
    while (cmd->args[i])
    {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            i++;
            continue;
        }

        if (pid == 0) {
            ft_execute(cmd->args[i], env);
        }
        i++;
    }

    // Parent waits for all children
    while (wait(NULL) > 0)
        ;
}

