/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:41 by abrami            #+#    #+#             */
/*   Updated: 2025/03/25 10:51:55 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// static int	count_word(char **s)
// {
// 	int	count;

// 	count = 0;
// 	while (*s)
// 	{
//         s++;
//         count++;
// 	}
// 	return (count);
// }

int     ft_strcmp(char *srt, char *str)
{
        int     i;

        i = 0;
        while (str[i] && srt[i] && str[i] == srt[i])
                i++;
        return (str[i] - srt[i]);
}
int main()
{
    char    *input;
    char    **search;
    pid_t   pid;

    while (1)
    {
        input = readline("minishell ");
        if (!input)
        {
            ft_printf("exit\n");
            exit(0);
        }
        if (ft_strcmp(input, "exit") == 0)
        {
            ft_printf("exit\n");
            free(input);
            break;
        }
        if (*input)
            add_history(input);
        search = ft_split(input, ' ');
        // ft_printf("%s\n", input);
        pid = fork();
        if (pid == 0)
            exec(search);
        else if (pid > 0)
            wait (0);
        else
            write (2, "Error\n", 6);
        // for(int i = 0; i < count_word(search); i++){
        //     ft_printf("%s\n", search[i]);
        // }
        free(input);
    }
    rl_clear_history();
    return (0);
}
