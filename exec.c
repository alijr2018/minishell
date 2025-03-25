/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:01 by abrami            #+#    #+#             */
/*   Updated: 2025/03/25 11:01:18 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *ft_return(char *full_path, char *dir, char **paths, char *str)
{
    int i;

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

static char *searchexec(char *str)
{
    char    *path;
    char    *full_path;
    char    *dir;
    char    **paths;
    int     i;

    path = getenv("PATH");
    if(!str || !*str)
        return (NULL);
    if (ft_strchr(str, '/'))
    {
        if (access(str, X_OK) == 0)
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
    // i = 0;
    // while(paths[i])
    // {
    //     full_path = malloc(strlen(paths[i]) + strlen(str) + 2);
    //     if (!full_path)
    //     {
    //         i++;
    //         continue;
    //     }
    //     strcpy(full_path, paths[i]);
    //     strcat(full_path, "/");
    //     strcat(full_path, str);
    //     if (access(full_path, X_OK) == 0)
    //         return (ft_strdup(full_path));
    //     free(full_path);
    //     i++;
    // }
    ft_return(full_path,dir, paths, str);
    // return (NULL);
}

void exec(char **alt)
{
    char *exec_path;

    if (ft_strcmp(*alt , "cd") == 0)
    {
        ft_printf("hello");
        return ;
    }
    exec_path = searchexec(*alt);
    if (!exec_path)
    {
        ft_printf("Command not found: %s\n", *alt);
        exit(127);
    }
    if (execve(exec_path, alt, NULL) == -1)
        perror("Error executing file");
    exit(1);
}