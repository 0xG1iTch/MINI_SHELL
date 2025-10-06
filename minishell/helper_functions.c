/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:03:43 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/09 10:48:13 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

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

char	*ftt_strjoin(t_shell *minishell, char const *s1, char const *s2)
{
	char	*new_s;
	size_t	total_len;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	else if (s1 == NULL)
		return (ft_strdup(s2));
	else if (s2 == NULL)
		return (ft_strdup(s1));
	total_len = ft_strlen(s1) + ft_strlen(s2);
	new_s = ft_alloc(total_len + 1, &minishell->collector, 0);
	if (!new_s)
		return (NULL);
	cpy_strs(new_s, s1, s2);
	return (new_s);
}

int	count_words(const char *s, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (s && s[i])
	{
		if (s[i] == c)
		{
			while (s[i] && s[i] == c)
				i++;
		}
		else
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

int	count_words2(const char *s)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (s && s[i])
	{
		if (s[i] == 32 || s[i] == 9)
		{
			while (s[i] && (s[i] == 32 || s[i] == 9))
				i++;
		}
		else
		{
			count++;
			while (s[i] && (s[i] != 32 && s[i] != 9))
				i++;
		}
	}
	return (count);
}

char	**ftt_split_expand(t_shell *minishell, char const *s)
{
	char	**f;
	int		total_words;

	if (!s)
		return (NULL);
	total_words = count_words2(s);
	f = ft_alloc((total_words + 1) * sizeof(char *), &minishell->collector, 0);
	if (!f)
		return (NULL);
	if (split_words_expand(minishell, f, s) == 0)
		return (NULL);
	return (f);
}
