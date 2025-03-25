/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:04:50 by abrami            #+#    #+#             */
/*   Updated: 2025/03/25 10:52:04 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_words(const char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static char	*get_word(const char *s, char c)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static char	**free_array(char **array, int i)
{
	while (i > 0)
	{
		free(array[--i]);
	}
	free(array);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	int		i;
	char	**array;

	i = 0;
	if (!s)
		return (NULL);
	array = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			array[i] = get_word(s, c);
			if (!array[i])
				return (free_array(array, i));
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	array[i] = NULL;
	return (array);
}
