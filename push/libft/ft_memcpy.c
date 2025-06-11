/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:06:45 by sellyani          #+#    #+#             */
/*   Updated: 2024/11/13 18:06:03 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	counter;

	if (src == dst)
		return (dst);
	counter = 0;
	while (counter < n)
	{
		*((unsigned char *)dst + counter) = *((unsigned char *)src + counter);
		counter++;
	}
	return (dst);
}
