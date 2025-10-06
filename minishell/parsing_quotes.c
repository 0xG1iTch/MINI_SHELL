/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:01:46 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/13 18:32:47 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_quotes	*add_quotes_back(t_quotes *new, t_quotes *head)
{
	t_quotes	*courant;

	courant = head;
	new->next = NULL;
	if (head == NULL)
		head = new;
	else
	{
		while (courant && courant->next)
			courant = courant->next;
		courant->next = new;
	}
	return (head);
}

t_quotes	*add_to_quotes_list(t_shell *ms, char *str, size_t i, int count)
{
	t_quotes	*new;
	int			j;

	j = 0;
	new = ft_alloc(sizeof(t_quotes), &ms->collector, 0);
	if (!new)
		free_and_flee(ms, 1);
	new->arg = ft_alloc(count + 1, &ms->collector, 0);
	if (!new->arg)
		free_and_flee(ms, 1);
	while (j < count)
	{
		new->arg[j] = str[i - count];
		j++;
		i++;
	}
	new->arg[j] = '\0';
	ms->head = add_quotes_back(new, ms->head);
	return (ms->head);
}

void	free_quotes_list(t_quotes *head)
{
	t_quotes	*courant;
	t_quotes	*curr;

	courant = head;
	while (courant)
	{
		curr = courant;
		courant = courant->next;
		free(curr->arg);
		free(curr);
	}
}
