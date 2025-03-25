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

static void	ft_run(char **search)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		exec(search);
	else if (pid > 0)
		wait (0);
	else
		write (2, "Error\n", 6);
}

int	main(void)
{
	char	*input;
	char	**search;

	while (1)
	{
		input = readline("minishell! ");
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
		ft_run(search);
		free(input);
	}
	rl_clear_history();
	return (0);
}
