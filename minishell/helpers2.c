/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:22:13 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/14 11:45:16 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_issue_redir(int count, char c)
{
	if (c == '>')
	{
		if (count % 2 == 0)
			printf("minishell: syntax error near unexpected token `>>'\n");
		else
			printf("minishell: syntax error near unexpected token `>'\n");
	}
	else if (c == '<')
	{
		if (count % 2 != 0)
			printf("minishell: syntax error near unexpected token `<'\n");
		else
			printf("minishell: syntax error near unexpected token `<<'\n");
	}
	return (-1);
}

int	count_redir(char *command, int i)
{
	int		count;
	char	c;

	c = command[i];
	count = 0;
	while (command[i] && command[i] == c)
	{
		count++;
		i++;
	}
	return (count);
}

int	redir_handler_helper(t_shell *ms, t_token *new, int i, int count)
{
	if (ms->command[i] == '<' && count == 1)
	{
		i = set_cmd(ms, ms->command, new, i);
		new->op = 1006;
	}
	else if (ms->command[i] == '>' && count == 1 \
		&& is_valid(ms->command[i + 1], 0))
	{
		i = set_cmd(ms, ms->command, new, i);
		new->op = 1005;
	}
	else if (ms->command[i] == '$' || ms->command[i + 1] == '$')
		i = set_cmd(ms, ms->command, new, i);
	return (i);
}

int	redir_handler(t_shell *ms, char *command, int i, t_token *new)
{
	int	count;

	count = count_redir(command, i);
	if (command[i] == '>' && count == 2 && is_valid(command[i + 2], 0))
	{
		i++;
		new->op = 1007;
		i = set_cmd(ms, command, new, i);
	}
	else if (command[i] == '<' && count == 2 && is_valid(command[i + 2], 0))
	{
		i++;
		new->op = 1008;
		i = set_cmd(ms, command, new, i);
	}
	else if (((command[i] == '<' || command[i] == '>') && count == 1 && \
	is_valid(command[i + 1], 0)) || (command[i] == '$' || \
	command[i + 1] == '$'))
		i = redir_handler_helper(ms, new, i, count);
	else
		return (ms->exit_status = 2, syntax_issue_redir(count, command[i]));
	return (i);
}
