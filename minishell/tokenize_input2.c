/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:56:43 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/07 11:32:02 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quotes(char *command, int *i)
{
	char	quote_type;

	quote_type = command[*i];
	(*i)++;
	while (command[*i] != '\0')
	{
		if (command[*i] == quote_type)
		{
			(*i)++;
			if (command[*i] == '"' || command[*i] == '\'')
			{
				quote_type = command[*i];
				(*i)++;
			}
			else
				break ;
		}
		else
			(*i)++;
	}
	return (*i);
}

int	calculating_len(char *command, int i)
{
	while (command[i] != '\0')
	{
		if (command[i] == '"' || command[i] == '\'')
			i = handle_quotes(command, &i);
		else if (command[i] == 32 || command[i] == '>' || command[i] == '|' \
			|| command[i] == '<' || command[i] == 9)
			break ;
		else
			i++;
	}
	return (i);
}

void	expanding_op(t_token *new)
{
	int	i;

	i = 0;
	while (new->cmd[i])
	{
		if (new->cmd[i] == '$')
			new->op = 1011;
		i++;
	}
}

t_token	*init_cmd(char *command, int *i, t_shell *minishell)
{
	t_token	*new;
	int		len;
	int		old_i;

	new = ft_alloc(sizeof(t_token), &minishell->collector, 0);
	if (!new)
		free_and_flee(minishell, 1);
	old_i = *i;
	new->op = 0;
	if (command[*i] == '-')
		new->op = 1;
	(*i) = calculating_len(command, (*i));
	len = *i - old_i;
	if (len <= 0)
		free_and_flee(minishell, 1);
	(*i)--;
	new->cmd = ft_alloc(sizeof(char) * (len + 1), &minishell->collector, 0);
	if (!new->cmd)
		free_and_flee(minishell, 1);
	ft_strlcpy(new->cmd, command + old_i, len + 1);
	new->next = NULL;
	expanding_op(new);
	return (new);
}
