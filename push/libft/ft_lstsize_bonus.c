/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:06:34 by sellyani          #+#    #+#             */
/*   Updated: 2024/10/30 15:40:47 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *head)
{
	int		len;
	t_list	*temp;

	len = 0;
	if (!head)
		return (0);
	temp = head;
	while (temp)
	{
		len++;
		temp = temp->next;
	}
	return (len);
}
