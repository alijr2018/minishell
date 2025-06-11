#include "minishell.h"

void update_underscore_with_path(t_shell *shell, const char *full_path)
{
    int i = 0;
    char *new_var;

    if (!full_path)
        return;

    new_var = ft_strjoin("_=", full_path);
    if (!new_var)
        return;

    while (shell->copy_env[i])
    {
        if (ft_strncmp(shell->copy_env[i], "_=", 2) == 0)
        {
            free(shell->copy_env[i]);
            shell->copy_env[i] = new_var;
            return;
        }
        i++;
    }

    shell->copy_env[i] = new_var;
    shell->copy_env[i + 1] = NULL;
}


void	builtin_env(t_shell *shell)
{
	int	i;

	if (shell->cmd[1])
	{
		printf("env: %s: No such file or directory\n", shell->cmd[1]);
		(shell->exit_status = 1);
		return;
	}
	i = 0;
	while (shell->copy_env[i])
	{
		printf("%s\n", shell->copy_env[i]);
		i++;
	}
	(shell->exit_status) = 0;
}
char **copy_env_vide(char **env)
{
    char *path = getcwd(NULL, 0);
    char *full_path = ft_strjoin("PWD=", path);
    env = append_env(env, full_path);
    free(path);
    free(full_path);
    return (env);
}
char **copy_envp(char **envp)
{
    int i = 0;
    int len = 0;
    char **env_copy;

    if (!envp || !(*envp))
    {
        env_copy = NULL;
        return (copy_env_vide(env_copy));
    }

    while (envp[len])
        len++;
    env_copy = malloc(sizeof(char *) * (len + 1));
    if (!env_copy)
        return NULL;

    while (i < len)
    {
        env_copy[i] = ft_strdup(envp[i]);
        i++;
    }
    env_copy[i] = NULL; 
    return env_copy;
}



