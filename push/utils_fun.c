#include "minishell.h"


long long ft_atoll_with_overflow(const char *str, int *invalid)
{
    long long result = 0;
    int sign = 1;
    int i = 0;

    *invalid = 0;

    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;

    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }

    if (!(str[i] >= '0' && str[i] <= '9'))
    {
        *invalid = 1;
        return 0;
    }

    while (str[i] && (str[i] >= '0' && str[i] <= '9'))
    {
        int digit = str[i] - '0';

        if (sign == 1)
        {
            if (result > (LLONG_MAX - digit) / 10)
            {
                *invalid = 1;  // overflow +
                return 0;
            }
        }
        else
        {
            if (result > ((unsigned long long)(-(LLONG_MIN + digit))) / 10)
            {
                *invalid = 1;  // overflow -
                return 0;
            }
        }
        result = result * 10 + digit;
        i++;
    }

    if (str[i] != '\0')
    {
        *invalid = 1;
        return 0;
    }

    return sign * result;
}


int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}


void clean_tmp_files(int max_id) {
    int i = 0;
    while (i < max_id) {
        char *num = ft_itoa(i);
        char *filename = ft_strjoin(".heredoc_tmp_", num);
        if (filename) {
            unlink(filename);
            free(filename);
        }
        free(num);
        i++;
    }
}


char **append_env(char **env, const char *new_var)
{
    int len = 0;
    if (env != NULL && env[len] != NULL)
        while (env && env[len])
            len++;
    char **new_env = malloc(sizeof(char *) * (len + 2));
    if (!new_env)
        return NULL;

    for (int i = 0; i < len; i++)
        new_env[i] = ft_strdup(env[i]);

    new_env[len] = ft_strdup(new_var);
    new_env[len + 1] = NULL;

    free_array(env);
    return new_env;
}
int is_numeric(const char *str)
{
    if (!str || !*str)
        return 0;
    int i = 0;
    if (str[0] == '-' || str[0] == '+')
        i++;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}

void handle_shlvl(t_shell *shell)
{
    int i = 0;
    int lvl = 1;
    char *new_env;

    while (shell->copy_env && shell->copy_env[i])
    {
        if (ft_strncmp(shell->copy_env[i], "SHLVL=", 6) == 0)
        {
            char *val = shell->copy_env[i] + 6;

            if (is_numeric(val))
            {
                lvl = ft_atoi(val) + 1;
                if (lvl > 999){
                    printf("minishell: warning: shell level (1000) too high, resetting to 1\n");
                    lvl = 1;
                }
                else if (lvl < 0)
                    lvl = 0;
            }
            else
                lvl = 1;

            char *new_val = ft_itoa(lvl);
            new_env = ft_strjoin("SHLVL=", new_val);

            free(shell->copy_env[i]);
            shell->copy_env[i] = new_env;

            free(new_val);
            return;
        }
        i++;
    }

    // إذا لم يتم العثور على SHLVL، نضيفه كـ SHLVL=1
    shell->copy_env = append_env(shell->copy_env, "SHLVL=1");
}


