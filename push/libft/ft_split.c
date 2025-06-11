/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:48:08 by sellyani          #+#    #+#             */
/*   Updated: 2024/11/13 18:00:15 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(const char *str, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (*str)
	{
		if (*str != c && j == 1)//said ellyani de mamn
		{
			j = 0;
			i++;
		}
		else if (*str == c && j == 0)
			j = 1;
		str++;
	}
	return (i);
}

static char	*ft_p1(const char *str, size_t start, size_t end)
{
	char	*s;
	int		i;

	i = 0;
	s = ft_calloc((end - start + 1), (sizeof(char)));
	if (!s)
		return (NULL);
	while (start < end)
	{
		s[i] = str[start];
		i++;
		start++;
	}
	s[i] = '\0';
	return (s);
}

char	**ft_split(char const *s, char c)
{
	size_t		i;
	size_t		j;
	int			debut_cpy;
	char		**src;

	i = -1;
	j = 0;
	debut_cpy = -1;
	if (!s)
		return (0);
	src = ft_calloc((ft_count_words(s, c) + 1), sizeof(char *));
	if (!src)
		return (NULL);
	while (++i <= ft_strlen(s))
	{
		if (s[i] != c && debut_cpy < 0)
			debut_cpy = i;
		else if ((s[i] == c || i == ft_strlen(s)) && debut_cpy >= 0)
		{
			src[j++] = ft_p1(s, debut_cpy, i);
			debut_cpy = -1;
		}
	}
	src[j] = 0;
	return (src);
}
