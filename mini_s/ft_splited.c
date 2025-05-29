/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splited.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:04:50 by abrami            #+#    #+#             */
/*   Updated: 2025/05/29 18:07:34 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #include <stdlib.h>
// #include <ctype.h>  // for isspace

// Checks if a character is a delimiter
static int	is_delim(char c)
{
	return (isspace((unsigned char)c) || c == '\'' || c == '"');
}

static int	count_words(const char *s)
{
	int	count = 0;

	while (*s)
	{
		while (*s && is_delim(*s))
			s++;
		if (*s)
		{
			count++;
			while (*s && !is_delim(*s))
				s++;
		}
	}
	return (count);
}

static char	*get_word(const char *s)
{
	int		len = 0;
	char	*word;
	int		i;

	while (s[len] && !is_delim(s[len]))
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
		free(array[--i]);
	free(array);
	return (NULL);
}

char	**ft_splited(const char *s)
{
	int		i = 0;
	char	**array;

	if (!s)
		return (NULL);
	array = malloc((count_words(s) + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	while (*s)
	{
		while (*s && is_delim(*s))
			s++;
		if (*s)
		{
			array[i] = get_word(s);
			if (!array[i])
				return (free_array(array, i));
			i++;
			while (*s && !is_delim(*s))
				s++;
		}
	}
	array[i] = NULL;
	return (array);
}

