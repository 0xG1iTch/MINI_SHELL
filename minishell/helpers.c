/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:33:03 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/13 18:34:04 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_lst_no_env(t_shell *ms, char *variable, char *content)
{
	t_env	*new;

	new = (t_env *)ft_alloc(sizeof(t_env), &ms->collector, 0);
	if (!new)
		return (NULL);
	new->full_env = ftt_strjoin(ms, variable, "=");
	new->full_env = ftt_strjoin(ms, new->full_env, content);
	new->variable = ftt_strdup(variable, ms);
	new->content = ftt_strdup(content, ms);
	new->next = NULL;
	return (new);
}

t_env	*new_lst(t_shell *minishell, char *env, char *variable, char *content)
{
	t_env	*new;

	new = (t_env *)ft_alloc(sizeof(t_env), &minishell->collector, 0);
	if (!new)
		return (NULL);
	new->full_env = (env);
	new->variable = (variable);
	new->content = content;
	new->next = NULL;
	return (new);
}

t_token	*init_token(t_shell *minishell, char *command, int *i)
{
	t_token	*new;
	int		flag;

	flag = 0;
	new = ft_alloc(sizeof(t_token), &minishell->collector, 0);
	if (!new)
		free_and_flee(minishell, 1);
	if (command[*i] == '|' && command[*i + 1] != '|')
		new->op = 1000;
	else if (command[*i] == '|' && command[*i + 1] == '|')
		1 && (new->op = 1004, (*i)++);
	else if (command[*i] == '&' && command[*i + 1] == '&')
		1 && (new->op = 1010, (*i)++);
	else if (command[*i] == ';')
		new->op = 1009;
	else
	{
		(*i) = redir_handler(minishell, command, *i, new);
		if (*i == -1)
			return (NULL);
		flag = 1;
	}
	if (flag == 0)
		new->cmd = NULL;
	return (new->next = NULL, new);
}
