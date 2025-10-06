/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:39:49 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/06 12:03:43 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	cpy_strs(char *new_s, const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < ft_strlen(s1))
	{
		new_s[i] = s1[i];
		i++;
	}
	while (j < ft_strlen(s2))
		new_s[i++] = s2[j++];
	new_s[i] = '\0';
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_s;
	size_t	total_len;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	total_len = ft_strlen(s1) + ft_strlen(s2);
	new_s = malloc(total_len + 1);
	if (!new_s)
		return (NULL);
	cpy_strs(new_s, s1, s2);
	return (new_s);
}
