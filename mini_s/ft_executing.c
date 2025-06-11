/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:01 by abrami            #+#    #+#             */
/*   Updated: 2025/06/11 12:02:23 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern  int g_var;

//this is the file that execute the command not the ones that should create

static void free_str_array(char **arr)
{
    int i = 0;
    if (!arr)
        return;
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
    if (!*env)
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
    if (!new_var || !strchr(new_var, '='))
        return env;

    char *equal_sign = strchr(new_var, '=');
    size_t key_len = equal_sign - new_var;
    int count = 0;
    int found = -1;

    // Count and search
    while (env && env[count]) {
        if (found == -1 && strncmp(env[count], new_var, key_len) == 0 && 
            env[count][key_len] == '=') {
            found = count;
        }
        count++;
    }

    // Allocate new array
    char **new_env = malloc(sizeof(char *) * (count + (found == -1 ? 2 : 1)));
    if (!new_env)
        return env;

    // Copy variables
    int j = 0;
    for (int i = 0; i < count; i++) {
        if (i == found) {
            new_env[j++] = strdup(new_var);
        } else {
            new_env[j++] = strdup(env[i]);
        }
    }

    // Add new if not found
    if (found == -1) {
        new_env[j++] = strdup(new_var);
    }
    new_env[j] = NULL;

    // Free old environment properly
    if (env) {
        for (int i = 0; env[i]; i++) {
            free(env[i]);  // Free each string
        }
        free(env);  // Free the array
    }

    return new_env;
}

static int	num_length(int n)
{
	int	len;

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

char	*ft_itoa(int n)
{
	int				len;
	char			*str;
	unsigned int	x;

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


static char *get_env_value(const char *name, char **env)
{
	if (!name)
		return (NULL);
	size_t len = strlen(name);
	for (int i = 0; env && env[i]; i++)
	{
		if (strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
	}
	return ("");
}

static char *expand_variables(const char *str, char **env)
{
	if (!str)
		return (NULL);

	char *result = calloc(1, 1);
	for (size_t i = 0; str[i];)
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				char *status = ft_itoa(g_var);
				char *tmp = ft_strjoin(result, status);
				free(result);
				free(status);
				result = tmp;
				i += 2;
			}
			else
			{
				size_t j = i + 1;
				while (str[j] && (isalnum(str[j]) || str[j] == '_'))
					j++;
				char *varname = strndup(str + i + 1, j - (i + 1));
				char *value = get_env_value(varname, env);
				char *tmp = ft_strjoin(result, value);
				free(result);
				free(varname);
				result = tmp;
				i = j;
			}
		}
		else
		{
			size_t len = strlen(result);
			char *tmp = realloc(result, len + 2);
			if (!tmp)
				break;
			tmp[len] = str[i];
			tmp[len + 1] = '\0';
			result = tmp;
			i++;
		}
	}
	return result;
}


int export_builtin(char **args, char ***env) 
{
    if (!args || !args[0]) {
        print_env_bash_format(*env);
        return 0;
    }

    for (int i = 0; args[i]; i++) {
        if (strchr(args[i], '=')) {
            *env = add_to_env(*env, args[i]);
        } else {
            // Handle invalid export syntax (no '=')
            fprintf(stderr, "export: invalid syntax: %s\n", args[i]);
            return 1;
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


// add part before here to check for builtin and env etc..
void ft_executing(t_command *cmd, char **env)
{
    int i = 0;
    int status;
    pid_t pid;

    if (!cmd || !cmd->args)
        return;
        
    for (int j = 0; cmd->args[j]; j++)
    {
        if (cmd->args[j][0] == '$') // Only expand if it starts with $
        {
            char *expanded = expand_variables(cmd->args[j], env);
            free(cmd->args[j]);
            cmd->args[j] = expanded;
        }
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
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			g_var = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_var = 128 + WTERMSIG(status);
		else
			g_var = 0;
	}
    free_str_array(cmd->args);
}

