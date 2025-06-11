#include "minishell.h"

void builtin_pwd(t_shell *shell) {
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd == NULL) {
        // Fallback to the stored PWD in our environment
        char *env_pwd = get_env_value(shell, "PWD");
        if (env_pwd) {
            printf("%s\n", env_pwd);
            shell->exit_status = 0;
        } else {
            perror("pwd");
            shell->exit_status = 1;
        }
    } else {
        printf("%s\n", cwd);
        free(cwd);
        shell->exit_status = 0;
    }
}