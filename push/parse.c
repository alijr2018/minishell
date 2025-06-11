/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:17:01 by abrami            #+#    #+#             */
/*   Updated: 2025/06/11 18:26:51 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_char_array(char **arr)
{
    int i = 0;
    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

// lexing then parsing 
//then each command add it to a linked list
// void parses(t_shell *shell, char *input)
// {
    
    
// }
// {
//     char *path;

//     if (ft_strchr(input, '|'))
//     {
//         shell->cmd = ft_splited(input);  //splited function
//         if (shell->cmd) {
//             exec_pipeline(shell->cmd, shell);
//             free_3d_array(shell->cmd);
//         }
//     }
//     else
//     {
//         // Handle regular command
//         shell->cmd = ft_token(input);
//         if (!shell->cmd) {
//             free(input);
//             return 0;
//         }

//         if (process_heredoc(shell->cmd) == -1) {
//             free_array(shell->cmd);
//             free(input);
//             return 0;
//         }

//         if (shell->cmd[0] && is_builtin(shell->cmd[0])) {
//             handl_builting(shell);
//         } else {
//             path = get_path(shell);
//             if (!path) {
//                 write(2, "minishell: ", 11);
//                 write(2, shell->cmd[0], ft_strlen(shell->cmd[0]));
//                 write(2, ": command not found\n", 21);
//                 shell->exit_status = 127;
//             } else {
//                 shell->exit_status = ft_exec(shell);
//                 free(path);
//             }
//         }
//         free_array(shell->cmd);
//     }
//     return 0;
// }

// int i = 0;
// while(shell->cmd[i])
// 	printf("parsed:%s\n", shell->cmd[i++]);