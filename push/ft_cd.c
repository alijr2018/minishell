#include "minishell.h"

char *get_env_value(t_shell *shell, const char *key)
{
    int     i;
    size_t key_len;

    if (!shell || !key)
        return NULL;

    key_len = ft_strlen(key);
    i = 0;
    while (shell->copy_env && shell->copy_env[i])
    {
        if (ft_strncmp(shell->copy_env[i], key, key_len) == 0 &&
            shell->copy_env[i][key_len] == '=')
        {
            return shell->copy_env[i] + key_len + 1;
        }
        i++;
    }
    return NULL;
}

static char *jn_dir(char *baze,const char *arg)
{
    char *tempo;
    char *rez;
    size_t ln;

    if (!arg && baze)
        return ft_strdup(baze);
    if (!arg && !baze)
        return NULL;
    if (arg[0] == '/')
        return ft_strdup(arg);
    if (!baze || baze[0] == '\0')
        return ft_strdup(arg);

    ln = ft_strlen(baze);
    if (ln > 0 && baze[ln - 1] == '/')
        return ft_strjoin(baze, arg);

    tempo = ft_strjoin(baze, "/");
    if (!tempo)
        return NULL;
    rez = ft_strjoin(tempo, arg);
    free(tempo);
    return rez;
}

static void update_pwd_vars(t_shell *shell, char *oldpwd_path, const char *path_arg)
{
    char *cwd = getcwd(NULL, 0);
    char *pwd_str;
    char *oldpwd_str;

    if (oldpwd_path)
    {
        oldpwd_str = ft_strjoin("OLDPWD=", oldpwd_path);
        add_or_update_env(shell, oldpwd_str);
        free(oldpwd_str);
    }

    if (cwd)
    {
        pwd_str = ft_strjoin("PWD=", cwd);
        add_or_update_env(shell, pwd_str);
        free(pwd_str);
        free(cwd);
    }
    else
    {
        ft_putstr_fd("minishell: cd: error retrieving current directory: ", 2);
        ft_putendl_fd(strerror(errno), 2);
        // في حالة فشل getcwd، احسب PWD يدويًا
        char *pwd_guess = jn_dir(get_env_value(shell, "PWD"), path_arg);
        if (pwd_guess)
        {
            pwd_str = ft_strjoin("PWD=", pwd_guess);
            add_or_update_env(shell, pwd_str);
            free(pwd_str);
            free(pwd_guess);
        }
    }
}

static void update_pwd_to_parent(t_shell *shell)
{
    char *pwd_val = get_env_value(shell, "PWD");
    if (!pwd_val)
        return;

    char *parent = ft_strdup(pwd_val);
    char *last_slash = ft_strrchr(parent, '/');
    if (last_slash && last_slash != parent)
        *last_slash = '\0';
    else
    {
        free(parent);
        parent = ft_strdup("/");
    }

    char *oldpwd_str = ft_strjoin("OLDPWD=", pwd_val);
    char *pwd_str = ft_strjoin("PWD=", parent);

    add_or_update_env(shell, oldpwd_str);
    add_or_update_env(shell, pwd_str);

    free(oldpwd_str);
    free(pwd_str);
    free(parent);
}

void builtin_cd(t_shell *shell)
{
    struct stat path_stat;
    char *oldpwd;

    if (!shell->cmd[1])
    {
        ft_putstr_fd("cd: missing argument\n", 2);
        shell->exit_status = 1;
        return;
    }

    oldpwd = getcwd(NULL, 0);

    if (stat(shell->cmd[1], &path_stat) == -1)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(shell->cmd[1], 2);
        ft_putstr_fd(": ", 2);
        ft_putstr_fd(strerror(errno), 2);
        ft_putstr_fd("\n", 2);
        free(oldpwd);
        shell->exit_status = 1;
        return;
    }

    if (!S_ISDIR(path_stat.st_mode))
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(shell->cmd[1], 2);
        ft_putstr_fd(": Not a directory\n", 2);
        free(oldpwd);
        shell->exit_status = 1;
        return;
    }

    if (chdir(shell->cmd[1]) == -1)
{
    if (ft_strcmp(shell->cmd[1], "..") == 0)
    {
        update_pwd_to_parent(shell);
        shell->exit_status = 0;
        free(oldpwd);
        return;
    }

    ft_putstr_fd("minishell: cd: ", 2);
    ft_putstr_fd(shell->cmd[1], 2);
    ft_putstr_fd(": ", 2);
    ft_putstr_fd(strerror(errno), 2);
    ft_putstr_fd("\n", 2);
    free(oldpwd);
    shell->exit_status = 1;
    return;
}


    update_pwd_vars(shell, oldpwd, shell->cmd[1]);
    free(oldpwd);
    shell->exit_status = 0;
}
