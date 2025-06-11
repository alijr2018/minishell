#include "minishell.h"

void builtin_exit(t_shell *comd)
{
    int invalid = 0;
    long long status;

    printf("exit\n");

    if (comd->cmd[1])
    {
        status = ft_atoll_with_overflow(comd->cmd[1], &invalid);
        if (invalid)
        {
            ft_putstr_fd("minishell: exit: ", 2);
            ft_putstr_fd(comd->cmd[1], 2);
            ft_putstr_fd(": numeric argument required\n", 2);
            free_array(comd->cmd);
            exit(2);
        }
        if (comd->cmd[2])
        {
            ft_putstr_fd("minishell: exit: too many arguments\n", 2);
            comd->exit_status = 1;
            // free_array(comd->cmd);
            return;
        }
        free_array(comd->cmd);
        exit((int)status);
    }
    free_array(comd->cmd);
    exit(comd->exit_status);
}
