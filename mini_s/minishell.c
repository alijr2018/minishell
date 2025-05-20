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

//check with leaks
static void	ft_exit(char *input)
{
	// (void) input;
	printf("exit\n");
	if (input)
		free(input);
	rl_clear_history();
	exit (0);
}

//this function for listing to Ctrl-C
void	lis(int i)
{
	(void)i;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(void)
{
	char	*input;

	signal(SIGINT, lis);
	while (1)
	{
		input = readline("minishell!>> ");
		if (!input || ft_strcmp(input, "exit") == 0)
			ft_exit(input);
		if (*input)
			add_history(input);
		// printf("this is ---> %s :\n", input);
		ft_executing(&input);
	}
	rl_clear_history();
	return (0);
}
