// #include "minishell.h"

// static int handle_heredoc(char *delimiter)
// {
//     int pipefd[2];
//     char *line;

//     if (pipe(pipefd) == -1)
//         return (perror("pipe"), -1);

//     while (1)
//     {
//         line = readline("> ");
//         if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
//         {
//             free(line);
//             break;
//         }
//         write(pipefd[1], line, ft_strlen(line));
//         write(pipefd[1], "\n", 1);
//         free(line);
//     }
//     close(pipefd[1]);
//     return pipefd[0];
// }


// int apply_redirections(t_redir *redir_list)
// {
//     int fd;

//     while (redir_list)
//     {
//         if (redir_list->type == REDIR_INPUT)
//         {
//             fd = open(redir_list->filename, O_RDONLY);
//             if (fd < 0)
//                 return (perror(redir_list->filename), -1);
//             dup2(fd, STDIN_FILENO);
//             close(fd);
//         }
//         else if (redir_list->type == REDIR_OUTPUT)
//         {
//             fd = open(redir_list->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
//             if (fd < 0)
//                 return (perror(redir_list->filename), -1);
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//         else if (redir_list->type == REDIR_APPEND)
//         {
//             fd = open(redir_list->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
//             if (fd < 0)
//                 return (perror(redir_list->filename), -1);
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//         else if (redir_list->type == REDIR_HEREDOC)
//         {
//             fd = handle_heredoc(redir_list->filename);
//             if (fd < 0)
//                 return -1;
//             dup2(fd, STDIN_FILENO);
//             close(fd);
//         }
//         redir_list = redir_list->next;
//     }
//     return 0;
// }




