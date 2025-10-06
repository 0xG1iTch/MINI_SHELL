/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:53:56 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/21 14:19:02 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe_error(t_shell *m, t_token *courant)
{
	while (courant && courant->next)
	{
		if (courant->op == DOUBLE_AND || courant->op == OR)
			return (m->exit_status = 2, printf("minishell: Syntax error\n"), 1);
		courant = courant->next;
	}
	if (courant && ((courant->op == DOUBLE_AND || courant->op == OR) \
	|| (courant->op == 1000 && courant->next == NULL)))
		return (m->exit_status = 2, printf("minishell: Syntax error\n"), 1);
	return (0);
}

int	printing_syn_error(t_shell *m, t_token *courant)
{
	if (courant && courant->next && (courant->next->op == 1005 \
		|| courant->next->op == 1006 || courant->next->op == 1007 \
		|| courant->next->op == 1008))
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		m->exit_status = 2;
		return (1);
	}
	if (courant->op == 1000)
		return (m->exit_status = 2, \
			printf("minishell: syntax error near unexpected token `|'\n"), 1);
	return (0);
}

int	checking(t_token *courant)
{
	if ((courant->op == 1006 || courant->op == 1007 || courant->op == 1008) \
	&& courant->next && courant->next->op == 1000)
		return (printf("minishell: syntax error near \
unexpected token `|'\n"), 1);
	else
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (1);
	}
	return (0);
}

int	checking2(t_shell *m, t_token *courant)
{
	if (courant->next->op == 1005 || courant->next->op == 1006 \
		|| courant->next->op == 1007 || courant->next->op == 1008)
	{
		m->exit_status = 2;
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (1);
	}
	else
		return (printing_syn_error(m, courant->next));
}

int	check_syntax(t_shell *minishell)
{
	t_token	*courant;

	courant = minishell->token;
	if (check_quotes(minishell) == 1)
		return (1);
	if (courant && courant->op == 1000)
		return (minishell->exit_status = 2, \
			printf("minishell: syntax error near unexpected token `|'\n"), 1);
	while (courant)
	{
		if ((courant->op == 1005 || courant->op == 1006 \
			|| courant->op == 1007 || courant->op == 1008) \
			&& courant->cmd == NULL && checking(courant) == 1)
			return (minishell->exit_status = 2, 1);
		if (courant != minishell->token && courant->next \
			&& courant->cmd == NULL && courant->next->cmd == NULL)
			return (checking2(minishell, courant));
		courant = courant->next;
	}
	return (handle_pipe_error(minishell, minishell->token));
}
