# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdarg.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
//this just for tests


void	lis(int i)
{
	(void)i;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(void)
{
	char	*input;

	signal(SIGINT, lis);
	while (1)
	{

		input = readline("minishell!>> ");

		if (!input)
        {
            printf("sad\n");
        }
        if (*input)
            add_history(input);
    }
    rl_clear_history();
	return (0);

}
