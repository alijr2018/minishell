/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 17:26:03 by abrami            #+#    #+#             */
/*   Updated: 2025/03/25 08:58:37 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_putchar(char c)
{
	write (1, &c, 1);
	return (1);
}

static int	type_print(char c, va_list args)
{
	int	i;

	i = 0;
	if (c == 's')
		i = ft_putstr(va_arg(args, char *));
	return (i);
}

static int	format(char str)
{
	size_t	i;
	char	*a;

	a = "cspdiuxX%";
	i = 0;
	while (a[i])
	{
		if (a[i] == str)
			return (1);
		i++;
	}
	return (0);
}

int	printf(const char *in, ...)
{
	va_list	args;
	size_t	i;
	size_t	j;

	if (write(1, "", 0) < 0)
		return (-1);
	i = 0;
	j = 0;
	va_start (args, in);
	while (in[i])
	{
		if (in[i] == '%' && format(in[i + 1]))
		{
			j += type_print(in[i + 1], args);
			i++;
		}
		else if (in[i] != '%')
			j += ft_putchar(in[i]);
		i++;
	}
	va_end(args);
	return (j);
}
