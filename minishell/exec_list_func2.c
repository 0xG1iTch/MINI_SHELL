/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_list_func2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:17:46 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 20:01:24 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calc_arg(t_token **token_courant)
{
	t_token	*courant;
	int		count;

	count = 0;
	courant = *token_courant;
	if (courant->op == 1000)
		courant = courant->next;
	while (courant && courant->op != 1000)
	{
		if (courant->cmd && courant->op != 1005 \
			&& courant->op != 1006 && courant->op != 1007 \
			&& courant->op != 1008)
			count++;
		courant = courant->next;
	}
	if (courant == NULL || courant->op == 1000)
		*token_courant = courant;
	return (count);
}

void	redir_file_init(t_shell *minishell, t_redir *new, t_token *info)
{
	int	len;

	len = ft_strlen(info->cmd);
	new->file = ft_alloc(len + 1, &minishell->collector, 0);
	if (!new->file)
		free_and_flee(minishell, 1);
	ft_strlcpy(new->file, info->cmd, len + 1);
}

void	redir_add_back(t_shell *minishell, t_redir *new, \
	t_exec_list *exec_courant, t_token *info)
{
	t_redir	*courant;

	new->next = NULL;
	if (info->op == 1005)
		new->type = 1;
	else if (info->op == 1006)
		new->type = 2;
	else if (info->op == 1007)
		new->type = 3;
	else if (info->op == 1008)
		new->type = 4;
	if (new->type == 4)
		new->do_not_expand = !info->quotes_existed;
	else
		new->do_not_expand = 0;
	redir_file_init(minishell, new, info);
	if (exec_courant->redir_list == NULL)
		exec_courant->redir_list = new;
	else
	{
		courant = exec_courant->redir_list;
		while (courant->next)
			courant = courant->next;
		courant->next = new;
	}
}

void	init_redir_list(t_shell *minishell, \
	t_exec_list *exec_courant, t_token *token_courant)
{
	t_token	*courant;
	t_redir	*new;

	courant = token_courant;
	while (courant && courant->op != 1000)
	{
		if (courant->op == 1005 || courant->op == 1006 \
			|| courant->op == 1007 || courant->op == 1008)
		{
			new = ft_alloc(sizeof(t_redir), &minishell->collector, 0);
			if (!new)
				free_and_flee(minishell, 1);
			redir_add_back(minishell, new, exec_courant, courant);
		}
		courant = courant->next;
	}
}

void	create_big_list(t_shell *ms)
{
	int			nodes_count;
	t_token		*courant;
	t_exec_list	*new;

	ms->executing = ft_alloc(sizeof(t_exec_list), &ms->collector, 0);
	if (!ms->executing)
		free_and_flee(ms, 1);
	ms->executing = NULL;
	courant = ms->token;
	nodes_count = 0;
	while (courant)
	{
		if (courant->op == 1000)
			nodes_count++;
		courant = courant->next;
	}
	while (nodes_count >= 0)
	{
		new = ft_alloc(sizeof(t_exec_list), &ms->collector, 0);
		if (!new)
			free_and_flee(ms, 1);
		big_list_add_back(ms, new);
		nodes_count--;
	}
}
