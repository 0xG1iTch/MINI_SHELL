/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_func3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:03:57 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 20:01:32 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_flag(t_token *curr, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (curr->quotes_flag[i] != 'e')
			curr->quotes_flag[i] = 'c';
		i++;
	}
	curr->quotes_flag[i] = '\0';
}

void	setting_flag(t_shell *ms, t_token *curr)
{
	size_t	len;
	char	*old_flag;

	old_flag = NULL;
	if (!curr || !curr->cmd)
		return ;
	len = ft_strlen(curr->cmd);
	if (curr->quotes_flag)
	{
		old_flag = ft_alloc(ft_strlen(curr->quotes_flag), &ms->collector, 0);
		if (!old_flag)
			free_and_flee(ms, 1);
		ft_strlcpy(old_flag, curr->quotes_flag, ft_strlen(curr->quotes_flag));
	}
	if (!curr->quotes_flag || ft_strlen(curr->quotes_flag) < len)
	{
		curr->quotes_flag = ft_alloc(len + 1, &ms->collector, 0);
		if (!curr->quotes_flag)
			free_and_flee(ms, 1);
	}
	if (old_flag)
		ft_strlcpy(curr->quotes_flag, old_flag, ft_strlen(old_flag));
	copy_flag(curr, len);
}

void	update_quotes_flag(t_shell *ms, t_token *c, \
size_t start_pos, size_t end_pos)
{
	if (!ms || !c || !c->cmd)
		return ;
	setting_flag(ms, c);
	if (!c->quotes_flag || start_pos >= end_pos || \
	end_pos > strlen(c->cmd) || end_pos == 0)
		return ;
	while (start_pos != end_pos)
	{
		c->quotes_flag[start_pos] = 'e';
		start_pos++;
	}
}

void	expanding_helper(t_shell *shell, t_token *c)
{
	size_t	i;
	int		in_single_quote;
	int		in_double_quote;
	size_t	start_pos;
	size_t	end_pos;

	i = 0;
	1 && (in_single_quote = 0, in_double_quote = 0, c->quotes_flag = NULL);
	setting_flag(shell, c);
	while (c && c->cmd[i])
	{
		expanding_quotes_cases(c, &in_single_quote, &in_double_quote, i);
		if (c->cmd[i] == '$' && c->cmd[i + 1] && is_valid(c->cmd[i + 1], 2) \
		&& !in_single_quote)
		{	
			1 && (start_pos = i, i++);
			end_pos = normal_expanding(&i, c, shell) + start_pos;
			update_quotes_flag(shell, c, start_pos, end_pos);
			c->got_expanded = 1;
		}
		else
			i++;
		if (ft_strlen(c->cmd) <= i)
			break ;
	}
}
