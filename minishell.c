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

int ft_strcmp(char *srt, char *str)
{
	int	i;

	i = 0;
	while (str[i] && srt[i] && str[i] == srt[i])
		i++;
	return (str[i] - srt[i]);
}

t_command *parse_single_command(char *segment) {
    t_command *cmd = malloc(sizeof(t_command));
    cmd->args = malloc(sizeof(char *) * MAX_ARGS);
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->heredoc_delimiter = NULL;
    cmd->append_output = 0;

    char **tokens = ft_split(segment, ' ');
    int i = 0, j = 0;

    while (tokens[i]) {
        if (!strcmp(tokens[i], "<") && tokens[i + 1]) {
            cmd->input_file = ft_strdup(tokens[++i]);
        } else if (!strcmp(tokens[i], "<<") && tokens[i + 1]) {
            cmd->heredoc_delimiter = ft_strdup(tokens[++i]);
        } else if (!strcmp(tokens[i], ">") && tokens[i + 1]) {
            cmd->output_file = ft_strdup(tokens[++i]);
            cmd->append_output = 0;
        } else if (!strcmp(tokens[i], ">>") && tokens[i + 1]) {
            cmd->output_file = ft_strdup(tokens[++i]);
            cmd->append_output = 1;
        } else {
            cmd->args[j++] = ft_strdup(tokens[i]);
        }
        i++;
    }
    cmd->args[j] = NULL;
    free(tokens);
    return cmd;
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

void handle_input(t_command *cmd) {
    if (cmd->heredoc_delimiter) {
        int pipefd[2];
        pipe(pipefd);
        char *line;
        while (1) {
            line = readline("> ");
            if (!line || !strcmp(line, cmd->heredoc_delimiter)) {
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
    } else if (cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);
        if (fd == -1) {
            perror("input");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
}

void handle_output(t_command *cmd) {
    if (cmd->output_file) {
        int fd = open(cmd->output_file,
                      O_WRONLY | O_CREAT | (cmd->append_output ? O_APPEND : O_TRUNC), 0644);
        if (fd == -1) {
            perror("output");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void exec_command(t_command *cmd)
{

    handle_input(cmd);
    handle_output(cmd);
    exec(cmd->args); //the problem is in echo
    // if (execvp(cmd->args[0], cmd->args) == -1) {
    //     perror("execvp");
    //     exit(127);
    // }
}

void execute_pipeline(char *input) {
    char *line = strdup(input);
    char *segment;
    int n = 0;
    t_command *commands[128];

    // Split pipeline segments
    segment = strtok(line, "|");
    while (segment) {
        commands[n++] = parse_single_command(segment);
        segment = strtok(NULL, "|");
    }

    int i, pipefd[2], in_fd = 0;
    pid_t pid;

    for (i = 0; i < n; i++) {
        if (i < n - 1) pipe(pipefd);

        pid = fork();
        if (pid == 0) {
            if (i > 0) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (i < n - 1) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            exec_command(commands[i]);
        } else {
            if (i > 0) close(in_fd);
            if (i < n - 1) {
                close(pipefd[1]);
                in_fd = pipefd[0];
            }
            waitpid(pid, NULL, 0);
        }
    }
    free(line);
    for (int j = 0; j < n; j++) {
        free(commands[j]);
    }
}

void    lis(int i)
{
    (void)i;
    ft_printf("\n");  
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}
int	main(void)
{
    char	*input;
	// char	**search;
    
	signal(SIGINT, lis);
	while (1)
	{
		input = read_full_command();
		if (!input || ft_strcmp(input, "exit") == 0)
		{
			ft_exit(input);
			exit(0);
		}
		if (*input)
			add_history(input);
		// search = ft_split(input, ' ');
        execute_pipeline(input);
		// handle_redirections_and_pipes(search);
		// ft_run(search);
		free(input);
	}
	rl_clear_history();
	return (0);
}
