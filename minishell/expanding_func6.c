/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_func6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:46:20 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 20:03:48 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_expand(t_shell *ms)
{
	int	i;

	i = 0;
	while (ms->command[i])
	{
		if (ms->command[i] == '$')
			return (0);
		i++;
	}
	return (1);
}

int	expanding_input_helper(t_shell *shell)
{
	t_token	*current;	

	current = shell->token;
	while (current)
	{
		if (current->op != 1000 && current->op != 1008)
		{
			if (current->op == 1005 || current->op == 1006 \
				|| current->op == 1007)
			{
				if (expand_variable(shell, current) == 1)
					return (1);
			}
			else
			{
				if (expand_variable(shell, current) == 1)
					return (1);
			}
		}
		current = splitting_check(shell, current);
		if (!current)
			break ;
		current = current->next;
	}
	return (0);
}

void	rebuilding_cmd(t_shell *ms, t_token *curr, t_quotes *head)
{
	t_quotes	*courant2;

	curr->cmd = NULL;
	curr->cmd = ft_alloc(ft_strlen(head->arg) + 1, &ms->collector, 0);
	if (!curr->cmd)
		free_and_flee(ms, 1);
	ft_strlcpy(curr->cmd, head->arg, ft_strlen(head->arg) + 1);
	courant2 = head->next;
	while (courant2)
	{
		curr->cmd = ftt_strjoin(ms, curr->cmd, courant2->arg);
		courant2 = courant2->next;
	}
}
