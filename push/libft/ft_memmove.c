/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:06:50 by sellyani          #+#    #+#             */
/*   Updated: 2024/11/11 17:05:31 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*led1;
	unsigned char	*led2;
	size_t			i;

	if (src == dst)
		return (dst);
	led1 = (unsigned char *)src;
	led2 = (unsigned char *)dst;
	if (led2 < led1)
		ft_memcpy(led2, led1, len);
	else
	{
		i = 1;
		while (i <= len)
		{
			led2[len - i] = led1[len - i];
			i++;
		}
	}
	return (led2);
}
