#include "minishell.h"

int contains_redirection(char **cmd)
{
    int i = 0;
    if (!cmd)
        return 0;
    while (cmd[i]) {
        if (!ft_strcmp(cmd[i], "<") || 
            !ft_strcmp(cmd[i], ">") || 
            !ft_strcmp(cmd[i], ">>") ||
            !ft_strcmp(cmd[i], "<<"))
            return 1;
        i++;
    }
    return 0;
}

char ***parse_pipes(char *line)
{
    char **segments = ft_split(line, '|');
    int count = 0;
    while (segments[count])
        count++;

    char ***cmds = malloc(sizeof(char **) * (count + 1));
    if (!cmds)
        return NULL;

    for (int i = 0; i < count; i++) {
        cmds[i] = ft_token(segments[i]);
    }
    cmds[count] = NULL;

    free_array(segments);
    return cmds;
}

void free_3d_array(char **arr) {
    int i = 0;
    if (!arr)
        return;
    while (arr[i]) {
        free_array(&arr[i]);
        i++;
    }
    free(arr);
}

int exec_pipeline(char **cmds, t_shell *shell)
{
    int i = 0, pipefd[2], prev_fd = -1;
    pid_t pid;
    int status = 0;

    while (cmds[i]) {
        if (process_heredoc(&cmds[i]) == -1)
            return 1;

        if (pipe(pipefd) == -1){
            perror("pipe failed");
            return 1;
        }
        pid = fork();
        if (pid == -1){
            perror("fork failed");
            return (1);
        }
        if (pid == 0) {
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (cmds[i + 1] != NULL)
                dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);

            if (contains_redirection(&cmds[i])) {
                if (handle_redirection(NULL, &cmds[i]) == -1)
                    exit(1);
            }

            shell->cmd = &cmds[i];
            if (is_builtin(&cmds[i][0])) {
                handl_builting(shell);
                exit(0);
            }

            char *path = get_path(shell);
            if (!path) {
                write(2, "minishell: ", 11);
                write(2, &cmds[i][0], ft_strlen(&cmds[i][0]));
                write(2, ": command not found\n", 21);
                exit(127);
            }
            ft_exec(shell);
            // execve(path, &cmds[i], shell->copy_env);
            // perror("execve failed");
            // exit(1);
        }

        if (prev_fd != -1)
            close(prev_fd);
        close(pipefd[1]);
        prev_fd = pipefd[0];
        i++;
    }

    while (i--)
        wait(&status);

    shell->exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1; //tenery !!!
    return 0;
}

// int main(int ac, char **av, char **env)
// {
//     char *line;
//     t_shell shell;
//     char *path;

//     (void)ac;
//     (void)av;

//     shell.copy_env = copy_envp(env);
//     shell.exit_status = 0;
//     shell.invalid_keys = NULL;
//     shell.invalid_count = 0;

//     handle_shlvl(&shell);

//     signal(SIGINT, sigint_handl);
//     signal(SIGQUIT, sigquit_handler);

//     while (1)
//     {
//         line = readline("$> ");
//         if (!line)
//             break;

//         if (line[0] == '\0') {
//             free(line);
//             continue;
//         }
//         add_history(line);

//         if (ft_strchr(line, '|')) {
//             char ***cmds = parse_pipes(line);
//             if (cmds) {
//                 exec_pipeline(cmds, &shell);
//                 free_3d_array(cmds);
//             }
//         } else {
//             shell.cmd = ft_token(line);
//             if (!shell.cmd) {
//                 free(line);
//                 continue;
//             }
//             if (process_heredoc(shell.cmd) == -1) {
//                 free_array(shell.cmd);
//                 free(line);
//                 continue;
//             }
//             if (shell.cmd[0] && is_builtin(shell.cmd[0])) {
//                 handl_builting(&shell);
//             } else {
//                 path = get_path(&shell);
//                 if (!path) {
//                     write(2, "minishell: ", 11);
//                     write(2, shell.cmd[0], ft_strlen(shell.cmd[0]));
//                     write(2, ": command not found\n", 21);
//                     shell.exit_status = 127;
//                 } else {
//                     shell.exit_status = ft_exec(&shell);
//                     free(path);
//                 }
//             }
//             free_array(shell.cmd);
//         }
//         free(line);
//     }

//     free_array(shell.copy_env);
//     rl_clear_history();
//     return shell.exit_status;
// }
// int main(int ac, char **av, char **env)
// {
//     char *line;
//     t_shell shell;

//     (void)ac;
//     (void)av;

//     shell.copy_env = copy_envp(env);
//     shell.exit_status = 0;
//     shell.invalid_keys = NULL;
//     shell.invalid_count = 0;

//     handle_shlvl(&shell);

//     signal(SIGINT, sigint_handl);
//     signal(SIGQUIT, sigquit_handler);

//     while (1) {
//         line = readline("$> ");
//         if (!line) {
//             // if (cmd.args != NULL)
//             //     free_char_array(cmd.args);
//             printf("exit\n");
//             rl_clear_history();
//             exit(0);
//         }

//         if (line[0] == '\0') {
//             free(line);
//             continue;
//         }

//         add_history(line);
        
//         // Now call parses() instead of having separate code for pipes and regular commands
//         parses(&shell, line);

//         free(line);
//     }

//     free_array(shell.copy_env);
//     rl_clear_history();
//     return (shell.exit_status);
// }
