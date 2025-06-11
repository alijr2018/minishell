#include "minishell.h"

int ft_exec(t_shell *shell) {
    pid_t child;
    char *full_cmd;
    int status;

    full_cmd = get_path(shell);
    

    if (full_cmd && ft_strncmp(full_cmd, "k", 2) == 0){
        
        printf("minishell: %s: No such file or directory\n", shell->cmd[0]);
        return (127);
    }
    if (full_cmd && ft_strncmp(full_cmd, "IS_DIR", 7) == 0) {
        printf("minishell: %s: Is a directory\n", shell->cmd[0]);
        // free_array(shell->cmd);
        free(full_cmd);
        return (126);
    }

    if (!full_cmd) {
        printf("minishell: %s: command not found\n", shell->cmd[0]);
        return 127;
    }

    child = fork();
    if (child == 0) {
        // ✅ NOW we apply redirection ONLY in the child
        if (contains_redirection(shell->cmd)) {
            if (handle_redirection(shell, shell->cmd) == -1) {
                free(full_cmd);
                free_array(shell->cmd);
                exit(1);
            }
        } 

        if (execve(full_cmd, shell->cmd, shell->copy_env) == -1) {
            perror("minishell: execution failed");
            free(full_cmd);
            free_array(shell->cmd);
            exit(1);
        }
    } else if (child > 0) {
        waitpid(child, &status, 0);
        update_underscore_with_path(shell, full_cmd);
        free(full_cmd);
        // free_array(shell->cmd);
    } else {
        perror("fork failed");
        return 1;
    }

    return WEXITSTATUS(status);
}

