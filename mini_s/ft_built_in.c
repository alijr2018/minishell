/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 14:36:52 by abrami            #+#    #+#             */
/*   Updated: 2025/06/08 15:49:16 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void change_dir(char *path, int print_path)
{
    char cwd[4096];

    if (!path)
        return;

    if (!getcwd(cwd, sizeof(cwd)))
        return;

    if (chdir(path) == 0)
    {
        if (print_path)
        {
            printf("%s", path);
            printf("\n");
        }
        setenv("OLDPWD", cwd, 1);
    }
    else
    {
        printf("cd: ");
        if (access(path, F_OK) == -1)
            printf("no such file or directory: ");
        else if (access(path, R_OK) == -1)
            printf("permission denied: ");
        else
            printf("not a directory: ");
        printf("%s", path);
    }
}

char *ft_strreplace(const char *str, const char *old, const char *new)
{
    char *pos = strstr(str, old);
    if (!pos)
        return NULL;

    size_t len_before = pos - str;
    size_t len_after = strlen(pos + strlen(old));
    size_t total_len = len_before + strlen(new) + len_after + 1;

    char *result = malloc(total_len);
    if (!result)
        return NULL;

    strncpy(result, str, len_before);
    result[len_before] = '\0';
    strcat(result, new);
    strcat(result, pos + strlen(old));

    return result;
}
int has_two_args(char **args)
{
    char cwd[4096];
    char *replaced;

    if (args[1] && args[2])
    {
        printf("cd: too many arguments\n");
        return 1;
    }

    if (args[1])
    {
        if (!getcwd(cwd, sizeof(cwd)))
            return 1;

        replaced = ft_strreplace(cwd, args[0], args[1]);
        if (!replaced)
        {
            printf("cd: string not in pwd: \n");
            printf("%s\n", args[0]);
            return 1;
        }

        change_dir(replaced, 1);
        free(replaced);
        return 1;
    }

    return 0;
}
int cd_builtin(char **args)
{
    char *home = getenv("HOME");
    char *oldpwd;

    if (!args[0]) // cd
    {
        change_dir(home, 0);
        return 1;
    }

    if (has_two_args(args))
        return 1;

    if (strcmp(args[0], "--") == 0)
    {
        change_dir(home, 0);
        return 1;
    }

    if (strcmp(args[0], "-") == 0)
    {
        oldpwd = getenv("OLDPWD");
        if (oldpwd)
            change_dir(oldpwd, 1);
        else
            printf("cd: OLDPWD not set\n");
        return 1;
    }

    change_dir(args[0], 0);
    return 1;
}

static void echo_out(char **str, int pos)
{
	int starts_with_quote;
	int ends_with_quote;
	int str_len;

	starts_with_quote = IS_QUOTE(str[pos][0]);
	str_len = (int)ft_strlen(str[pos]);
	ends_with_quote = IS_QUOTE(str[pos][str_len - 1]);

	if (starts_with_quote && ends_with_quote)
		ft_putnstr(str[pos] + 1, str_len - 2);  // Print without first and last quote
	else if (starts_with_quote)
		printf("%s", str[pos] + 1);
	else
		printf("%s",str[pos]);

	if (str[pos + 1])
		printf(" ");
}
int is_valid_n_flag(const char *arg)
{
    int i = 1;

    if (arg[0] != '-' || arg[1] != 'n')
        return 0;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}

int echo_builtin(char **args)
{
    int i = 0;
    int n_flag = 0;

    // Parse multiple -n flags
    while (args[i] && is_valid_n_flag(args[i]))
    {
        n_flag = 1;
        i++;
    }

    while (args[i])
    {
        echo_out(args, i);
        if (args[i + 1])
            printf(" ");
        i++;
    }

    if (!n_flag)
        printf("\n");

    return 1;
}

