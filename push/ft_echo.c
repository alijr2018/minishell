#include "minishell.h"

void    builtin_echo(t_shell *cmd){
	int	i;
	int	new_line;

	i = 1;
	new_line = 1;

	while (cmd->cmd[i] && ft_strncmp(cmd->cmd[i], "-n", 2) == 0 && cmd->cmd[i][2] == '\0'){
		new_line = 0;
		i++;
	}
	
	while (cmd->cmd[i])
	{
		printf("%s", cmd->cmd[i]);
		if (cmd->cmd[i + 1])
			printf(" ");
		i++;
	}
	if (new_line)
		printf("\n");
    (cmd->exit_status) = 0;
	
}