/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:01 by abrami            #+#    #+#             */
/*   Updated: 2025/05/27 16:30:03 by abrami           ###   ########.fr       */
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

void    ft_execute(char **alt, char **env)
{
    char    *exec_path;
    
    exec_path = searchexec(*alt, env);
    if (!exec_path)
    {
        //add free
        printf("Command not found: %s\n", *alt);
        exit(127);
    }
    if (execve(exec_path, alt, NULL) == -1)
    perror("Error executing file");
    exit(1);
}
// void    ft_executing(char **alt, char **env)
// {
        
// }