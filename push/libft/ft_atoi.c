/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:05:02 by sellyani          #+#    #+#             */
/*   Updated: 2025/02/13 14:28:32 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static void	ft(long *i, long *sign, char s)
{
	if (s == '-' || s == '+')
	{
		if (s == '-')



			*sign = -1;
		(*i)++;
	}
}

long	ft_atoi(const char *str)
{
	long	i;
	long	sign;
	long	result;



	
	
	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	ft(&i, &sign, str[i]);
	while (str[i] && ft_isdigit(str[i]))
	{
		result = (result * 10) + (str[i] - 48);
		i++;

		
	}
	return (result * sign);
}