/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_list_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:19:38 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/21 20:16:12 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	big_list_add_back(t_shell *minishell, t_exec_list *new)
{
	t_exec_list		*courant;

	courant = minishell->executing;
	new->cmd = NULL;
	new->args = NULL;
	new->redir_list = NULL;
	new->in = 0;
	new->out = 1;
	new->built_in = 0;
	new->flag = 0;
	new->next = NULL;
	if (minishell->executing == NULL)
		minishell->executing = new;
	else
	{
		while (courant && courant->next)
			courant = courant->next;
		courant->next = new;
		courant->next->next = NULL;
	}
}

static void	fill_exec_args(t_shell *minishell, \
	t_exec_list *exec, t_token *token, int n)
{
	int	i;
	int	flag;

	1 && (i = 0, flag = 0);
	while (exec->args && i < n)
	{
		if (token->cmd && token->op != 1005 && \
		token->op != 1006 && token->op != 1007 && token->op != 1008)
		{
			if (token->cmd)
			{
				exec->args[i] = ft_alloc(ft_strlen(token->cmd) + 1,
						&minishell->collector, 0);
				if (!exec->args[i])
					free_and_flee(minishell, 1);
				ft_strlcpy(exec->args[i], token->cmd, \
				ft_strlen(token->cmd) + 1);
				flag = 1;
			}
			i++;
		}
		token = token->next;
	}
	if (flag == 1)
		exec->args[i] = NULL;
}

static void	set_exec_cmd(t_shell *minishell, t_exec_list *exec)
{
	exec->cmd = ft_alloc(ft_strlen(exec->args[0]) + 1, \
	&minishell->collector, 0);
	if (!exec->cmd)
		free_and_flee(minishell, 1);
	ft_strlcpy(exec->cmd, exec->args[0], ft_strlen(exec->args[0]) + 1);
}

void	init_exec_node(t_shell *minishell, int n, \
	t_exec_list *exec, t_token *token)
{
	if (token->op == 1000)
		token = token->next;
	init_redir_list(minishell, exec, token);
	if (n > 0)
		exec->args = ft_alloc((n + 1) * sizeof(char *), \
		&minishell->collector, 0);
	if (n > 0 && !exec->args)
		free_and_flee(minishell, 1);
	fill_exec_args(minishell, exec, token, n);
	if (exec->args)
		set_exec_cmd(minishell, exec);
	exec->got_expanded = token->got_expanded;
}

int	command_list(t_shell *minishell)
{
	t_token		*token_courant;
	t_token		*old_token_courant;
	t_exec_list	*exec_courant;
	int			args_count;

	create_big_list(minishell);
	exec_courant = minishell->executing;
	token_courant = minishell->token;
	while (exec_courant)
	{
		old_token_courant = token_courant;
		args_count = calc_arg(&token_courant);
		init_exec_node(minishell, args_count, exec_courant, old_token_courant);
		exec_courant = exec_courant->next;
	}
	check_heredoc_limit(minishell);
	return (0);
}
