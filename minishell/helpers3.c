/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:22:27 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/13 18:30:52 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_cmd_redir_helper(t_shell *minishell, int *i, int len)
{
	int		inside_quotes;

	inside_quotes = 0;
	(*i)++;
	if (minishell->command[*i] == '>' || minishell->command[*i] == '<')
		(*i)++;
	while (minishell->command[*i] && (minishell->command[*i] == 32 \
		|| minishell->command[*i] == 9))
		(*i)++;
	len = *i;
	while (minishell->command[len] && minishell->command[len] != '<' \
	&& minishell->command[len] != '>' && minishell->command[len] != '|')
	{
		if (minishell->command[len] == 34 || minishell->command[len] == 39)
			inside_quotes = !inside_quotes;
		if (inside_quotes == 0 && (minishell->command[len] == 32 \
			|| minishell->command[len] == 9))
			break ;
		len++;
	}
	return (len);
}

static void	cmd_helper(char *c, int *i, char quote)
{
	while (c && c[*i])
	{
		if (c[*i] && c[*i] != quote)
			break ;
		(*i)++;
	}
	if (c[*i] == quote)
		(*i)++;
}

int	other_cmd_case(char *c, int *i)
{
	int		len;
	char	quote;

	len = *i;
	while (c && c[*i] && c[*i] != 32 && \
		c[*i] != '|' && c[*i] != '>' && c[*i] != '<')
	{
		if (c[*i] == 34 || c[*i] == 39)
		{
			quote = c[*i];
			(*i)++;
			cmd_helper(c, i, quote);
		}
		else if (c[*i] == '$')
		{
			(*i)++;
			while (c[*i] && c[*i] != 32 && c[*i] != '|' && \
				c[*i] != '>' && c[*i] != '<' && c[*i] != 34 && c[*i] != 39)
				(*i)++;
		}
		else
			(*i)++;
	}
	return (len);
}

int	setting_cmd_helper(t_shell *ms, t_token *new, int i, int len)
{
	if (i == len || (i == len + 2 && \
		(ms->command[len] == 34 || ms->command[len] == 39)))
		return (1);
	new->cmd = ft_alloc(i - len + 1, &ms->collector, 0);
	if (!new->cmd)
		free_and_flee(ms, 1);
	ft_strlcpy(new->cmd, ms->command + len, i - len + 1);
	return (0);
}

int	set_cmd(t_shell *minishell, char *command, t_token *new, int i)
{
	int		len;

	len = 0;
	while (command[i] && (command[i] == 32 || command[i] == 9))
		i++;
	if (!command[i] || command[i] == '|')
		return (new->cmd = NULL, i);
	else if (command[i] == '>' || command[i] == '<')
	{
		len = set_cmd_redir_helper(minishell, &i, len);
		if (len == i)
			new->cmd = NULL;
		else
		{			
			new->cmd = ft_alloc(len - i + 1, &minishell->collector, 0);
			if (!new->cmd)
				free_and_flee(minishell, 1);
			ft_strlcpy(new->cmd, minishell->command + i, len - i + 1);
		}
		return (len - 1);
	}
	len = other_cmd_case(command, &i);
	if (setting_cmd_helper(minishell, new, i, len) == 1)
		return (new->cmd = NULL, i - 1);
	return (i - 1);
}
