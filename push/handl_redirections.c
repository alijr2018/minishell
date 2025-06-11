#include "minishell.h"

int handle_redirection(t_shell *shell, char **cmd)
{
    int i = 0;
    int infile = -1;
    int outfile = -1;

    while (cmd[i])
    {
        if (ft_strncmp(cmd[i], "<", 2) == 0)
        {
            if (!cmd[i + 1])
                return (fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n"), -1);
            infile = open(cmd[i + 1], O_RDONLY);
            if (infile < 0)
                return (perror(cmd[i + 1]), -1);
            cmd[i] = NULL;
            cmd[i + 1] = NULL;
            i += 2;
        }
        else if (ft_strncmp(cmd[i], ">", 2) == 0)
        {
            if (!cmd[i + 1])
                return (fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n"), -1);
            outfile = open(cmd[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (outfile < 0)
                return (perror(cmd[i + 1]), -1);
            cmd[i] = NULL;
            cmd[i + 1] = NULL;
            i += 2;
        }
        else if (ft_strncmp(cmd[i], ">>", 3) == 0)
        {
            if (!cmd[i + 1])
                return (fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n"), -1);
            outfile = open(cmd[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (outfile < 0)
                return (perror(cmd[i + 1]), -1);
            cmd[i] = NULL;
            cmd[i + 1] = NULL;
            i += 2;
        }
        else
            i++;
    }

    if (infile != -1)
    {
        dup2(infile, STDIN_FILENO);
        close(infile);
    }
    if (outfile != -1)
    {
        dup2(outfile, STDOUT_FILENO);
        close(outfile);
    }

    return 0;
}
