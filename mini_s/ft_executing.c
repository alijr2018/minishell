/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:01 by abrami            #+#    #+#             */
/*   Updated: 2025/06/09 16:45:29 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern  int g_var;

//this is the file that execute the command not the ones that should create

static void free_str_array(char **arr)
{
    int i = 0;
    if (!arr) return;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}

static char	*searchexec(char *str, char **env)
{
    char    *path;
    char    *full_path, **paths;
    int     i;

    if(!str || !*str)
        return (NULL);
    if (!env)
        path = apath;
    
    for (int i = 0; env[i] != NULL; i++) {
        if (strncmp(env[i], "PATH=", 5) == 0) {
            path = env[i] + 5;  // Skip "PATH="
            break;
        }
    }
    if (ft_strchr(str, '/'))
    {
        // search failure for access 
        if (access(str, X_OK) == 0 && access(str, F_OK) == 0)
            return (ft_strdup(str));
        return (NULL);
    }

    full_path = malloc(strlen(str) + 3); // "./" + str + '\0'
    if (!full_path)
        return (NULL);
    strcpy(full_path, "./");
    strcat(full_path, str);
    if (access(full_path, X_OK) == 0 && access(full_path, F_OK) == 0)
        return (full_path);
    free(full_path);
    paths = ft_split(path, ':');
    if (!paths)
        return (NULL); // no need for free_str_array(paths) here

    i = 0;
    while (paths[i])
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
        {
            char *ret = ft_strdup(full_path);
            free(full_path);
            free_str_array(paths); // ✅ clean up ft_split result
            return ret;
        }
        free(full_path);
        i++;
    }
    free_str_array(paths);
    return NULL;

}

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
    {
        perror("Error executing file");
        free(exec_path);
        free(argv);
        exit(1);
    }
}

int	env_cmp(const void *a, const void *b)
{
	const char *s1 = *(const char **)a;
	const char *s2 = *(const char **)b;
	return strcmp(s1, s2);
}

// Print in bash-style format
void	print_env_bash_format(char **env)
{
	int count = 0;
	while (env[count])
		count++;

	// Duplicate and sort
	char **sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return;
	for (int i = 0; i < count; i++)
		sorted[i] = env[i];
	sorted[count] = NULL;

	qsort(sorted, count, sizeof(char *), env_cmp);

	for (int i = 0; i < count; i++)
	{
		char *equal = strchr(sorted[i], '=');
		if (equal)
		{
			*equal = '\0';
			printf("declare -x %s=\"%s\"\n", sorted[i], equal + 1);
			*equal = '='; // restore original
		}
		else
			printf("declare -x %s\n", sorted[i]);
	}
	free(sorted);
}

char **add_to_env(char **env, const char *new_var)
{
	int i = 0;
	char *key_end = strchr(new_var, '=');
	if (!key_end)
		return env;

	size_t key_len = key_end - new_var;
	while (env && env[i])
	{
		if (strncmp(env[i], new_var, key_len) == 0 && env[i][key_len] == '=')
		{
			// Don't free original env[i]; just duplicate the new one into a new array
			int j = 0;
			while (env[j])
				j++;

			char **new_env = malloc(sizeof(char *) * (j + 1));
			for (int k = 0; k < j; k++)
			{
				if (k == i)
					new_env[k] = strdup(new_var);
				else
					new_env[k] = strdup(env[k]);
			}
			new_env[j] = NULL;
			// Optional: free old env if you own it
			// free_str_array(env);
			return new_env;
		}
		i++;
	}

	// If key wasn't found, add new entry
	int count = 0;
	while (env && env[count])
		count++;
	char **new_env = malloc(sizeof(char *) * (count + 2));
	for (int k = 0; k < count; k++)
		new_env[k] = strdup(env[k]);
	new_env[count] = strdup(new_var);
	new_env[count + 1] = NULL;
	// Optional: free old env if you own it
	// free_str_array(env);
	return new_env;
}


int export_builtin(char **args, char ***env)
{
    if (!args || !args[0]) {
        print_env_bash_format(*env); // like bash
        return 0;
    }

    for (int i = 0; args[i]; i++) {
        if (strchr(args[i], '=')) {
            *env = add_to_env(*env, args[i]);  // store it
        }
    }
    return 0;
}


static int ft_check(char **args, char **env)
{
    if (!args || !args[0])
        return (1);

    if (strcmp(args[0], "cd") == 0)
    {
        cd_builtin(&args[1]);
        return (0);
    }
    else if (strcmp(args[0], "echo") == 0)
    {
        echo_builtin(&args[1]);
        return (0);
    }
    else if (strcmp(args[0], "export") == 0)
    {
        export_builtin(&args[1], &env); // pass env pointer
        return 0;
    }
    
    return (1);
}

static int      num_length(int n)
{
        int     len;

        len = 0;
        if (n <= 0)
                len = 1;
        while (n != 0)
        {
                n /= 10;
                len++;
        }
        return (len);
}

char    *ft_itoa(int n)
{
        int                             len;
        char                    *str;
        unsigned int    x;

        len = num_length(n);
        str = (char *)malloc(len + 1);
        if (!str)
                return (NULL);
        str[len] = '\0';
        if (n < 0)
        {
                x = -n;
                str[0] = '-';
        }
        else
        {
                x = n;
                str[0] = '0';
        }
        while (x > 0)
        {
                str[--len] = (x % 10) + '0';
                x /= 10;
        }
        return (str);
}
char *expand_exit_status(const char *arg)
{
    char *result;
    char *status_str = ft_itoa(g_var);  // convert int to string

    if (!status_str)
        return strdup(arg);  // fallback

    // allocate enough memory: input + possible status string + null
    size_t len = strlen(arg) + strlen(status_str) + 1;
    result = malloc(len);
    if (!result)
    {
        free(status_str);
        return strdup(arg);
    }

    const char *p = arg;
    char *r = result;

    while (*p)
    {
        if (*p == '$' && *(p + 1) == '?')
        {
            strcpy(r, status_str);
            r += strlen(status_str);
            p += 2;
        }
        else
            *r++ = *p++;
    }
    *r = '\0';
    free(status_str);
    return result;
}


// add part before here to check for builtin and env etc..
void ft_executing(t_command *cmd, char **env)
{
    int i = 0;
    pid_t pid;

    if (!cmd || !cmd->args)
        return;
    for (int j = 0; cmd->args[j]; j++)
    {
        char *expanded = expand_exit_status(cmd->args[j]);
        free(cmd->args[j]);
        cmd->args[j] = expanded;
    }
    if (!ft_check(cmd->args, env))
        return;
    while (cmd->args[i])
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            i++;
            continue;
        }
        if (pid == 0) 
            ft_execute(cmd->args[i], env);
        i++;
    }
    // Parent waits for all children
    // while (wait(NULL) > 0)
    //     ;
    int status;
    while (waitpid(-1, &status, 0) > 0)
    {
        if (WIFEXITED(status))
            g_var = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_var = 128 + WTERMSIG(status);  // Like bash
        else
            g_var = 1;  // default fallback
    }
    free_str_array(cmd->args);
}

