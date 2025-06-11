/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:07:53 by sellyani          #+#    #+#             */
/*   Updated: 2024/11/13 18:04:51 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	l1;
	size_t	l2;
	char	*s3;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return ((char *)s2);
	if (!s2)
		return ((char *)s1);
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	s3 = (char *)malloc(l1 + l2 + 1);
	if (!s3)
		return (NULL);
	ft_strlcpy(s3, s1, l1 + 1);
	ft_strlcat(s3, s2, l1 + l2 + 1);
	return (s3);
}
