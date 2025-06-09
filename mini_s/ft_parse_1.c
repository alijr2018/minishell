/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:49:54 by abrami            #+#    #+#             */
/*   Updated: 2025/06/09 16:07:46 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_stack *ft_lstnew(t_command **args, size_t index)
{
	t_stack *node = malloc(sizeof(t_stack));
	if (!node)
		return NULL;
	node->args = args;
	node->index = index;
	node->next = NULL;
	return node;
}

void add_back(t_stack **lst, t_stack *new_node)
{
	t_stack *tmp;

	if (!lst || !new_node)
		return;
	if (*lst == NULL)
	{
		*lst = new_node;
		return;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

void ft_lstclear(t_stack **lst)
{
	t_stack *temp;

	if (!lst || !*lst)
		return;

	while (*lst)
	{
		temp = *lst;
		*lst = (*lst)->next;

		if (temp->args && *temp->args)
		{
			t_command *cmd = *temp->args;
			if (cmd->args)
			{
				for (int i = 0; cmd->args[i]; i++)
					free(cmd->args[i]);
				free(cmd->args);
			}
			free(cmd);
		}
		free(temp->args);
		free(temp);
	}
	*lst = NULL;
}
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

int parses(t_command *cmd, char *input)
{

	cmd->args = ft_splited(input);  // your split function
	if (!cmd->args)
    {
        free_char_array(cmd->args);
		return (1);
    }
	// free_char_array(cmd->args);
	return (0);
}

// int i = 0;
// while(cmd->args[i])
// 	printf("parsed:%s\n", cmd->args[i++]);


