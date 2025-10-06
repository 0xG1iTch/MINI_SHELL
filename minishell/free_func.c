/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:49:24 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/24 18:44:43 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_flee(t_shell *minishell, int ac)
{
	minishell->collector = ft_alloc(0, &minishell->collector, 1);
	free(minishell->collector);
	free(minishell->command);
	free(minishell);
	rl_clear_history();
	if (ac == -1)
		return ;
	else
		exit (ac);
}

void	*free_gc(t_gc **collector)
{
	t_gc	*courant;
	t_gc	*temp;

	courant = *collector;
	while (courant)
	{
		temp = courant;
		courant = courant->next;
		free(temp->ptr);
		free(temp);
	}
	*collector = NULL;
	return (NULL);
}

void	*ft_alloc(size_t size, t_gc **collector, int ac)
{
	void	*new_ptr;
	t_gc	*new_node;

	if (!collector)
		return (NULL);
	if (ac == 0)
	{
		if (size <= 0)
			return (NULL);
		new_ptr = malloc(size);
		if (!new_ptr)
			return (NULL);
		new_node = (t_gc *)malloc(sizeof(t_gc));
		if (!new_node)
			return (free(new_ptr), NULL);
		new_node->ptr = new_ptr;
		new_node->next = *collector;
		*collector = new_node;
		return (new_ptr);
	}
	else if (ac == 1)
		return (free_gc(collector));
	return (NULL);
}

char	*ftt_strndup(const char *s1, size_t n, t_shell *ms)
{
	char	*dest;
	size_t	len;
	size_t	j;

	len = ft_strlen(s1);
	if (len > n)
		len = n;
	dest = ft_alloc(len + 1, &ms->collector, 0);
	if (!dest)
		return (NULL);
	j = 0;
	while (j < len && s1[j])
	{
		dest[j] = s1[j];
		j++;
	}
	dest[j] = '\0';
	return (dest);
}
