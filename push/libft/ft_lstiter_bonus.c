/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:06:11 by sellyani          #+#    #+#             */
/*   Updated: 2024/11/13 18:09:53 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *head, void (*f)(void *))
{
	t_list	*temp;

	if (!head)
		return ;
	temp = head;
	while (temp)
	{
		(*f)(temp->content);
		temp = temp->next;
	}
}
