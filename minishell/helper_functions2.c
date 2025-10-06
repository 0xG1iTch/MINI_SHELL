/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:03:49 by yel-haya          #+#    #+#             */
/*   Updated: 2025/04/26 15:08:20 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

char	*allocate_word(t_shell *minishell, const char *s, int start, int end)
{
	char	*word;

	word = ft_alloc(end - start + 1, &minishell->collector, 0);
	if (!word)
		return (NULL);
	ft_strlcpy(word, s + start, end - start + 1);
	return (word);
}

static void	free_memory(char **f, int i)
{
	while (i > 0)
		free(f[--i]);
	free(f);
}

static int	split_words(t_shell *minishell, char **f, const char *s, char c)
{
	int	endings;
	int	i;
	int	start;
	int	total_words;

	total_words = count_words(s, c);
	endings = 0;
	i = 0;
	while (i < total_words)
	{
		while (s[endings] == c)
			endings++;
		start = endings;
		while (s[endings] && s[endings] != c)
			endings++;
		f[i] = allocate_word(minishell, s, start, endings);
		if (!f[i])
		{
			free_memory(f, i);
			return (0);
		}
		i++;
	}
	f[i] = NULL;
	return (1);
}

char	**ftt_split(t_shell *minishell, char const *s, char c)
{
	char	**f;
	int		total_words;

	if (!s)
		return (NULL);
	total_words = count_words(s, c);
	f = ft_alloc((total_words + 1) * sizeof(char *), &minishell->collector, 0);
	if (!f)
		return (NULL);
	if (split_words(minishell, f, s, c) == 0)
		return (NULL);
	return (f);
}

int	split_words_expand(t_shell *minishell, char **f, const char *s)
{
	int	endings;
	int	i;
	int	start;
	int	total_words;

	total_words = count_words2(s);
	endings = 0;
	i = 0;
	while (i < total_words)
	{
		while (s[endings] == 9 || s[endings] == 32)
			endings++;
		start = endings;
		while (s[endings] && s[endings] != 9 && s[endings] != 32)
			endings++;
		f[i] = allocate_word(minishell, s, start, endings);
		if (!f[i])
		{
			free_memory(f, i);
			return (0);
		}
		i++;
	}
	f[i] = NULL;
	return (1);
}
