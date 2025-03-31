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
	ft_printf("exit\n");
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

void handle_pipes(char **args) {
    int i = 0;
    int pipe_fd[2];
    pid_t pid;
    int prev_pipe = -1; // This variable stores the previous pipe's read end

    while (args[i]) {
        // When we encounter the pipe
        if (strcmp(args[i], "|") == 0) {
            // Create a pipe
            if (pipe(pipe_fd) == -1) {
                perror("Pipe failed");
                exit(1);
            }

            pid = fork();
            if (pid == 0) {
                // Child process
                if (prev_pipe != -1) {
                    // Redirect the previous pipe's read end to stdin
                    dup2(prev_pipe, STDIN_FILENO);
                    close(prev_pipe);
                }
                close(pipe_fd[0]); // Close read end of current pipe
                dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to pipe
                args[i] = NULL; // Terminate the current command at the pipe
                ft_run(args); // Execute the command
            } else if (pid > 0) {
                // Parent process
                close(pipe_fd[1]); // Close write end of current pipe
                if (prev_pipe != -1) {
                    close(prev_pipe); // Close previous pipe's read end
                }
                prev_pipe = pipe_fd[0]; // Update the previous pipe's read end
                i++; // Move to the next command after the pipe
            } else {
                perror("Fork failed");
                exit(1);
            }
        } else {
            i++; // Keep moving until we find the pipe or end of commands
        }
    }

    // If no pipe was encountered, execute the last command (if any)
    if (prev_pipe != -1) {
        close(prev_pipe);
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0);
}


void handle_redirections_and_pipes(char **args)
{
    int i = 0;

    // Check for any pipes in the command
    handle_pipes(args);

    // After handling pipes, apply any redirections and run the command
    // handle_redirections(args);

    // If no pipe was present, execute the command normally
    ft_run(args);
}



int	main(void)
{
	char	*input;
	char	**search;
	
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
