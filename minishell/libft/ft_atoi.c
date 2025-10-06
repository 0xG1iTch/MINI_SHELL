/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-10-23 12:26:02 by mel-hamm          #+#    #+#             */
/*   Updated: 2024-10-23 12:26:02 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;
	long	checker;

	i = 0;
	sign = 1;
	result = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str++ == '-')
			sign = -1;
	}
	while (ft_isdigit(str[i]))
	{
		checker = result;
		result = result * 10 + (*str++ - '0');
		if (result / 10 != checker && sign == 1)
			return (-1);
		if (result / 10 != checker && sign == -1)
			return (0);
	}
	return (sign * result);
}
