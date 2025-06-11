/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:08:34 by sellyani          #+#    #+#             */
/*   Updated: 2024/11/13 18:02:34 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!haystack && needle && len == 0)
		return (0);
	while (haystack[i] != '\0' && i <= len)
	{
		j = 0;
		while (needle[j] != '\0' && (needle[j] == haystack[i + j])
			&& ((i + j) < len) && haystack[i + j] != '\0')
		{
			j++;
		}
		if (j == ft_strlen (needle))
			return ((char *)haystack + i);
		i++;
	}
	if ((ft_strlen(haystack) == 0) && ft_strlen(needle) == 0)
	{
		return ((char *)haystack);
	}
	return (NULL);
}
