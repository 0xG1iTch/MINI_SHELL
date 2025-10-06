/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-01 08:48:55 by mel-hamm          #+#    #+#             */
/*   Updated: 2024-11-01 08:48:55 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	size_t	slen;

	slen = ft_strlen(s);
	i = 0;
	while (i <= slen)
	{
		if (s[slen - i] == (char) c)
		{
			return ((char *)&s[slen - i]);
		}
		i++;
	}
	return (NULL);
}
