/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 02:05:11 by abrami            #+#    #+#             */
/*   Updated: 2025/05/27 02:14:39 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(t_command **args)
{
	t_command	*tmp;
	t_command	*current;

	if (!(*args))
		return ;
	current = *args;
	while (current->next != *args)
	{
		tmp = current;
		current = current->next;
		free(tmp);
	}
	free(current);
	*args = NULL;
}
void ft_free1(t_command cmd)
{
    if (cmd->args)
		free_args(&cmd->args);
	rl_clear_history();
}