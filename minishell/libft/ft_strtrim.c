/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-10-31 10:27:58 by mel-hamm          #+#    #+#             */
/*   Updated: 2024-10-31 10:27:58 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed;
	size_t	strt;
	size_t	end;

	if (s1 == NULL || set == NULL)
		return (NULL);
	strt = 0;
	end = ft_strlen(s1);
	while (s1[strt] && ft_strchr(set, s1[strt]))
		strt++;
	while (end > strt && ft_strchr(set, s1[end - 1]))
		end--;
	trimmed = malloc(end - strt + 1);
	if (trimmed == NULL)
		return (NULL);
	ft_strlcpy (trimmed, s1 + strt, end - strt + 1);
	return (trimmed);
}
