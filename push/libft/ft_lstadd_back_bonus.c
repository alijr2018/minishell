/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:05:50 by sellyani          #+#    #+#             */
/*   Updated: 2024/11/13 18:18:19 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **head, t_list *new_node)
{
	if (!head || new_node == NULL)
		return ;
	if (*head != NULL)
	{
		ft_lstlast (*head)->next = new_node;
	}
	else
	{
		*head = new_node;
	}
}
