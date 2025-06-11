#include "minishell.h"

static int is_valid_unset_var(char *var) {
    if (!var || !var[0] || ft_isdigit(var[0]))
        return 0;
    for (int i = 0; var[i]; i++) {
        if (!(ft_isalnum(var[i]) || var[i] == '_'))
            return 0;
    }
    return 1;
}

static void remove_from_env(char ***env, char *var) {
    int len = ft_strlen(var);
    int i = 0;
    while (*env && (*env)[i]) {
        if (ft_strncmp((*env)[i], var, len) == 0 &&
            ((*env)[i][len] == '=' || (*env)[i][len] == '\0')) {
            free((*env)[i]);
            while ((*env)[i + 1]) {
                (*env)[i] = (*env)[i + 1];
                i++;
            }
            (*env)[i] = NULL;
            return;
        }
        i++;
    }
}

void builtin_unset(t_shell *shell) {
    int i = 1;
    int status = 0;

    while (shell->cmd[i]) {
        if (is_valid_unset_var(shell->cmd[i])) {
            remove_from_env(&shell->copy_env, shell->cmd[i]);
            remove_from_env(&shell->export_env, shell->cmd[i]);
        } else {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(shell->cmd[i], 2);
            ft_putstr_fd("`: not a valid identifier\n", 2);
            status = 1;
        }
        i++;
    }
    shell->exit_status = status;
}
