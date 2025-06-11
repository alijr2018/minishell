/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:05:12 by sellyani          #+#    #+#             */
/*   Updated: 2024/11/13 17:56:07 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if ((int)count < 0 && (int)size < 0)
	{
		return (NULL);
	}
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero (ptr, size * count);
	return (ptr);
}
