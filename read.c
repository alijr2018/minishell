/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:06:52 by abrami            #+#    #+#             */
/*   Updated: 2025/04/07 15:50:10 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Return 1 if there's an unclosed quote
static int	has_unclosed_quotes(const char *str)
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
}

char	*read_full_command(void)
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
			break ;
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

void	ft_exit(char *input)
{
	ft_printf("exit\n");
	free(input);
}

// void    lol(int fd)
// {
// 	if (fd == -1) {
// 		perror("input");
// 		exit(1);
// 	}
// }
void handle_input(t_command *cmd)
{
	int		pipefd[2];
	char	*line;
	int		fd;
	
	if (cmd->heredoc_delimiter)
	{
		pipe(pipefd);
		while (1)
		{
			line = readline("> ");
			if (!line || !strcmp(line, cmd->heredoc_delimiter))
			{
				free(line);
				break;
			}
			write(pipefd[1], line, strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
	else if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1) {
			perror("input");
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	handle_output(t_command *cmd)
{
	int	fd;

	if (cmd->output_file)
	{
		fd = open(cmd->output_file,
			O_WRONLY | O_CREAT | (cmd->append_output ? O_APPEND : O_TRUNC), 0644);
		if (fd == -1)
		{
			perror("output");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
