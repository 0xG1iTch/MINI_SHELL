/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:29:08 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/14 10:57:15 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

t_quotes	*no_quotes(t_shell *ms, t_token *curr, size_t *i)
{
	int	count;

	count = 0;
	while (curr->cmd[*i] && curr->cmd[*i] != 34 && curr->cmd[*i] != 39)
	{
		(*i)++;
		count++;
	}
	ms->head = add_to_quotes_list(ms, curr->cmd, *i, count);
	return (ms->head);
}

t_quotes	*double_quotes(t_shell *ms, t_token *curr, size_t *i)
{
	int		count;
	char	c;

	count = 0;
	curr->quotes_existed = 1;
	c = curr->cmd[*i];
	(*i)++;
	while (curr->cmd[*i] && curr->cmd[*i] != c)
	{
		(*i)++;
		count++;
	}
	ms->head = add_to_quotes_list(ms, curr->cmd, *i, count);
	(*i)++;
	return (ms->head);
}

t_quotes	*single_quotes(t_shell *ms, t_token *curr, size_t *i)
{
	int		count;
	char	c;

	count = 0;
	curr->quotes_existed = 1;
	c = curr->cmd[*i];
	(*i)++;
	while (curr->cmd[*i] && curr->cmd[*i] != c)
	{
		(*i)++;
		count++;
	}
	ms->head = add_to_quotes_list(ms, curr->cmd, *i, count);
	(*i)++;
	return (ms->head);
}

static t_quotes	*helper_for_quotes(t_shell *ms, t_token *cur, size_t *i)
{
	if (cur->cmd[*i] == 34 && cur->quotes_flag[*i] != 'e')
	{
		ms->head = double_quotes(ms, cur, i);
	}
	else if (cur->cmd[*i] == 39 && cur->quotes_flag[*i] != 'e')
	{
		ms->head = single_quotes(ms, cur, i);
	}
	else
	{
		(*i)++;
		ms->head = add_to_quotes_list(ms, cur->cmd, *i, 1);
	}
	return (ms->head);
}

void	strip_out_quotes(t_shell *ms)
{
	t_token		*cur;
	size_t		i;

	cur = ms->token;
	ms->head = NULL;
	while (cur)
	{
		if (cur->cmd && cur->cmd[0] != '\0')
		{
			i = 0;
			while (cur->cmd[i])
			{
				if (cur->cmd[i] != 34 && cur->cmd[i] != 39)
					ms->head = no_quotes(ms, cur, &i);
				else
					ms->head = helper_for_quotes(ms, cur, &i);
				if (i > ft_strlen(cur->cmd))
					break ;
			}
			rebuilding_cmd(ms, cur, ms->head);
		}
		1 && (cur = cur->next, ms->head = NULL);
	}
}
