/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:15:51 by abrami            #+#    #+#             */
/*   Updated: 2025/05/27 16:29:22 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strcmp(char *src, char *dest)
{
    int i;

    i = 0;
    while(src[i] && dest[i] && src[i] == dest[i])
        i++;
    return (src[i] - dest[i]);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
		{
			return ((char *)&s[i]);
		}
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	return (NULL);
}
char	*ft_strdup(const char *s1)
{
	char	*ptr;
	int		i;

	i = 0;
	ptr = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (ptr == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
size_t  ft_strlen(const char *str)
{
    size_t  i;

    i = 0;
    while(str[i])
        i++;
    return (i);
}
char	*ft_strtok(char *str, const char *delim)
{
	static char	*next;
	char		*st;

	if (str)
		next = str;
	if (!next)
		return (NULL);
	while (*next && strchr(delim, *next))
		next++;
	if (*next == '\0')
		return (NULL);
	st = next;
	while (*next && !strchr(delim, *next))
		next++;
	if (*next)
	{
		*next = '\0';
		next++;
	}
	else
		next = (NULL);
	return (st);
}

static int	add_new_str(t_list **new, char *elem)
{
	(*new) = malloc(sizeof(t_list));
	if (*new == NULL)
		return (0);
	(*new)->str = elem;
	(*new)->next_one = NULL;
	(*new)->previous = NULL;
	return (1);
}

static void	add_first(t_list **list, t_list *new)
{
	(*list) = new;
	(*list)->previous = *list;
	(*list)->next_one = *list;
}

int add_to_list(t_list **list, char *str)
{
	t_list	*tmp;

	if (!add_new_str(&tmp, str))
		return (0);
	if (!(*list))
		add_first(list, tmp);
	else
	{
		tmp->previous = (*list)->previous;
		tmp->next_one = (*list);
		(*list)->previous->next_one = tmp;
		(*list)->previous = tmp;
	}
	return (1);
}