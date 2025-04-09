#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

// #define MAX_ARGS 1024

// typedef struct s_command {
//     char **args;
//     char *input_file;
//     char *output_file;
//     char *heredoc_delimiter;
//     int append_output;
// } t_command;

int ft_strcmp(char *srt, char *str)
{
    int i = 0;
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

void exec_command(t_command *cmd) {
    handle_input(cmd);
    handle_output(cmd);
    if (execvp(cmd->args[0], cmd->args) == -1) {
        perror("execvp");
        exit(127);
    }
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

int main(void) {
    char *input;

	signal(SIGINT, lis);

    while (1) {
        input = readline("minishell> ");
        if (!input)
		{
			exit(0);
		}
		if (ft_strcmp(input, "exit") == 0)
		{
			break ;
		}
        if (*input) add_history(input);

        execute_pipeline(input);
        free(input);
    }
    return 0;
}
