/* ************************************************************************** */
/*	                                                                          */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:41 by abrami            #+#    #+#             */
/*   Updated: 2025/03/25 11:15:40 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
**how my code work is get input with readline then pass directly to execve and then search for
** the command if it is run other wise command not found , to implimented |(pipe) and the other
**  i need to parse mean i have to search for the implimentation if i found it i have to run the
** command given diff way .
*/
int	ft_strcmp(char *srt, char *str)
{
	int	i;

	i = 0;
	while (str[i] && srt[i] && str[i] == srt[i])
		i++;
	return (str[i] - srt[i]);
}

static void	ft_exit(char *input)
{
	printf("exit\n");
	free(input);
}

int has_unclosed_quotes(const char *str)
{
    int	single_quote;
	int	double_quote;

    single_quote = 0;
	double_quote = 0;
	while (*str)
    {
        if (*str == '\'' && double_quote == 0)
            single_quote = !single_quote;
        else if (*str == '"' && single_quote == 0)
		double_quote = !double_quote;
	str++;
    }
    return (single_quote || double_quote);
	// Return 1 if there's an unclosed quote
}

char *read_full_command(void)
{
    char	*input;
    char	*temp;
    char	*line;

    input = NULL;
	input = readline("minishell!> ");
    if (!input)
		return (NULL);
	while (has_unclosed_quotes(input))
    {
        temp = readline("> ");
        if (!temp)
            break;
        line = malloc(ft_strlen(input) + ft_strlen(temp) + 2);
        if (!line)
            return (input);
		strcpy(line, input);
		strcat(line, "/");
		strcat(line, temp);
        free(input);
        input = line;
        free(temp);
    }
    return (input);
}
static void ft_run(char **search)
{
    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        exec(search); // Execute the command
        // perror("execve failed");
        // exit(1);
    }
    else if (pid > 0)
    {
        wait(NULL); // Wait for child process to finish
    }
    else
    {
        perror("Fork failed");
    }
}

void handle_pipes(char **args)
{
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;
    int i = 0;
    int start = 0;

    while (args[i])
    {
        if (strcmp(args[i], "|") == 0 || args[i + 1] == NULL)
        {
            int end;
            if (args[i + 1] == NULL)
                end = i + 1;
            else
                end = i;
            char *cmd[1024];
            int j = 0;
            for (int k = start; k < end; k++)
                cmd[j++] = args[k];
            cmd[j] = NULL;

            if (args[i + 1] != NULL && pipe(pipe_fd) == -1)
            {
                perror("pipe");
                exit(1);
            }

            pid = fork();
            if (pid == 0)
            {
                if (prev_fd != -1)
                {
                    dup2(prev_fd, STDIN_FILENO);
                    close(prev_fd);
                }
                if (args[i + 1] != NULL)
                {
                    close(pipe_fd[0]);
                    dup2(pipe_fd[1], STDOUT_FILENO);
                    close(pipe_fd[1]);
                }
                ft_run(cmd);
                exit(1);
            }
            else if (pid > 0)
            {
                if (prev_fd != -1)
                    close(prev_fd);
                if (args[i + 1] != NULL)
                {
                    close(pipe_fd[1]);
                    prev_fd = pipe_fd[0];
                }
            }
            else
            {
                perror("fork");
                exit(1);
            }
            start = i + 1;
        }
        i++;
    }
    while (wait(NULL) > 0);
}


void handle_redirections_and_pipes(char **args)
{
    handle_pipes(args);
}

void    lis(int i)
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
	char	**search;

	signal(SIGINT, lis);
	while (1)
	{
		input = read_full_command();
		if (!input)
		{
			ft_exit(input);
			exit(0);
		}
		if (ft_strcmp(input, "exit") == 0)
		{
			ft_exit(input);
			break ;
		}
		if (*input)
			add_history(input);
		search = ft_split(input, ' ');
		handle_redirections_and_pipes(search);
		// ft_run(search);
		free(input);
	}
	rl_clear_history();
	return (0);
}
