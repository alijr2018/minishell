#include "minishell.h"

int is_builtin(char *cmd) {
    return (ft_strncmp(cmd, "echo", 5) == 0 ||
            ft_strncmp(cmd, "cd", 3) == 0 ||
            ft_strncmp(cmd, "pwd", 4) == 0 ||
            ft_strncmp(cmd, "export", 7) == 0 ||
            ft_strncmp(cmd, "unset", 6) == 0 ||
            ft_strncmp(cmd, "env", 4) == 0 ||
            ft_strncmp(cmd, "exit", 5) == 0);
 }
    
 void handl_builting(t_shell *shell) {
    int saved_stdout = -1;

    if (contains_redirection(shell->cmd)) {
        saved_stdout = dup(STDOUT_FILENO);
        if (handle_redirection(shell, shell->cmd) == -1) {
            return;
        }
    }

    if (ft_strncmp(shell->cmd[0], "cd", 3) == 0) {
        builtin_cd(shell);
    } else if (ft_strncmp(shell->cmd[0], "pwd", 4) == 0) {
        builtin_pwd(shell);
    } else if (ft_strncmp(shell->cmd[0], "echo", 5) == 0) {
        builtin_echo(shell);
    } else if (ft_strncmp(shell->cmd[0], "export", 7) == 0) {
        builtin_export(shell);
    } else if (ft_strncmp(shell->cmd[0], "unset", 6) == 0) {
        builtin_unset(shell);
    } else if (ft_strncmp(shell->cmd[0], "env", 4) == 0) {
        builtin_env(shell);
    } else if (ft_strncmp(shell->cmd[0], "exit", 5) == 0) {
        builtin_exit(shell);
    }

    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
}
