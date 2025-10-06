/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:29:07 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 20:01:48 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_to_list(t_token *new, t_shell *minishell)
{
	t_token	*courant;

	if (minishell->token == NULL)
	{
		minishell->token = new;
		new->next = NULL;
		new->prec = NULL;
	}
	else
	{
		courant = minishell->token;
		while (courant->next)
			courant = courant->next;
		courant->next = new;
		new->prec = courant;
	}
	new->got_expanded = 0;
}

int	not_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != 32)
			return (1);
		i++;
	}
	return (0);
}

int	tokenize_input_helper(t_shell *ms, t_token *new, char *cmd, int *i)
{
	new = init_token(ms, cmd, i);
	if (!new)
		return (-1);
	new->do_not_expand = 0;
	new->quotes_existed = 0;
	if (new->op != 69)
		add_to_list(new, ms);
	return (1);
}

int	tokenize_input(char *cmd, t_shell *minishell)
{
	int		i;
	t_token	*new;
	int		len;
	int		flag;

	1 && (i = 0, len = ft_strlen(cmd));
	while (i < len && not_empty(cmd) == 1)
	{
		if (flag == -1)
			return (1);
		flag = 0;
		if (cmd[i] == '>' || cmd[i] == '|' || cmd[i] == '<' \
		|| (cmd[i] == '&' && cmd[i + 1] == '&') \
		|| cmd[i] == '$' || cmd[i + 1] == '$')
			flag = tokenize_input_helper(minishell, new, cmd, &i);
		else if (flag == 0 && cmd[i] != 32 && cmd[i] != 9)
		{
			new = init_cmd(cmd, &i, minishell);
			new->do_not_expand = 0;
			new->quotes_existed = 0;
			add_to_list(new, minishell);
		}
		i++;
	}
	return (0);
}
