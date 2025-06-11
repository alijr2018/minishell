#include "minishell.h"

static int write_heredoc_to_tmpfile(char *delimiter, char *filename) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
        return -1;

    char *line = NULL;
    while (1) {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0) {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    return 0;
}

int process_heredoc(char **cmd) {
    int i = 0;
    int tmp_id = 0;

    while (cmd[i])
    {
        if (ft_strncmp(cmd[i], "<<", 3) == 0)
        {
            if (!cmd[i + 1]) {
                write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
                return -1;
            }

            char *num = ft_itoa(tmp_id);
            char *filename = ft_strjoin(".heredoc_tmp_", num);
            free(num);

            if (!filename)
                return -1;

            if (write_heredoc_to_tmpfile(cmd[i + 1], filename) == -1) {
                free(filename);
                return -1;
            }

            free(cmd[i]);
            cmd[i] = ft_strdup("<");

            free(cmd[i + 1]);
            cmd[i + 1] = filename;

            tmp_id++;
            i += 2;
        } else {
            i++;
        }
    }
    return (tmp_id);
}