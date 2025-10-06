/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_func4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:20:31 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 20:04:05 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid(char c, int ac)
{
	if (ac == 0)
	{
		if (c == '>' || c == '<')
			return (0);
		return (1);
	}
	else
		return (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') \
			|| (c >= '0' && c <= '9') || c == '?');
}

int	quotes_exist(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (1);
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
			return (1);
		i++;
	}
	return (0);
}

int	expand_variable(t_shell *shell, t_token *curr)
{
	if (!curr->cmd)
	{
		curr->cmd = ft_alloc(1, &shell->collector, 0);
		curr->cmd[0] = '\0';
	}
	return (expand_work(curr, shell));
}

char	*get_env_value(t_shell *shell, char *var)
{
	t_env	*env;

	env = shell->envirement;
	while (env)
	{
		if (ft_strcmp(env->variable, var) == 0)
			return (env->content);
		env = env->next;
	}
	return ("");
}

int	inside_quotes(char *str)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	if (str == NULL)
		return (1);
	while (str[i])
	{
		if (str[i] == 34)
			in_double_quotes = !in_double_quotes;
		if (str[i] == 39)
			in_single_quotes = !in_single_quotes;
		if ((in_double_quotes == 1 || in_single_quotes == 1) \
		&& (str[i] == 32 || str[i] == 9))
			return (1);
		i++;
	}
	return (0);
}
