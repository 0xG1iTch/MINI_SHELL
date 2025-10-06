/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:55:23 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/14 11:49:51 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pile	*add_pile(t_shell *minishell, t_pile *p, char c)
{
	t_pile	*new;

	new = malloc(sizeof(t_pile));
	if (!new)
		free_and_flee(minishell, 1);
	new->str = c;
	new->next = p;
	return (new);
}

t_pile	*remove_pile(t_pile *p)
{
	t_pile	*tmp;

	if (!p)
		return (NULL);
	tmp = p;
	p = p->next;
	free(tmp);
	return (p);
}

int	quotes_succession(t_shell *minishell, char *str)
{
	t_pile	*p;
	int		i;

	i = 0;
	p = NULL;
	while (str[i])
	{
		if (str[i] == '"' && !p)
			p = add_pile(minishell, p, str[i]);
		else if (str[i] == '\'' && !p)
			p = add_pile(minishell, p, str[i]);
		else if (str[i] == '"' && p && p->str == '"')
			p = remove_pile(p);
		else if (str[i] == '\'' && p && p->str == '\'')
			p = remove_pile(p);
		i++;
	}
	i = (p == NULL);
	while (p)
		p = remove_pile(p);
	return (i);
}

int	check_quotes(t_shell *minishell)
{
	int	count1;
	int	count2;
	int	i;

	i = 0;
	count1 = 0;
	count2 = 0;
	while (minishell->command[i])
	{
		if (minishell->command[i] == 34 && count2 % 2 != 0)
			count1++;
		if (minishell->command[i] == 39 && count1 % 2 != 0)
			count2++;
		i++;
	}
	if (count1 % 2 != 0 || count2 % 2 != 0)
		return (minishell->exit_status = 2, \
			printf("Syntax error missing/invalid quote\n"), 1);
	if (quotes_succession(minishell, minishell->command) == 0)
		return (minishell->exit_status = 2, \
			printf("Syntax error missing/invalid quote\n"), 1);
	return (0);
}
