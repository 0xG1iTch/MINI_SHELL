/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:01:15 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/04 13:01:09 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	modify_env(char *var, char *value, t_shell *ms)
{
	t_env	*tmp;

	tmp = ms->envirement;
	while (tmp)
	{
		if (ft_strcmp(tmp->variable, var) == 0)
		{
			if (value)
			{
				tmp->content = ftt_strdup(value, ms);
				tmp->full_env = ftt_strjoin(ms, var, "=");
				tmp->full_env = ftt_strjoin(ms, tmp->full_env, value);
			}
			else
				tmp->full_env = ftt_strdup(var, ms);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	set_env(char *var, char *value, t_shell *ms)
{
	t_env	*new_node;

	if (modify_env(var, value, ms) == 0)
		return (0);
	new_node = ft_alloc(sizeof(t_env), &ms->collector, 0);
	if (!new_node)
		return (ft_putstr_fd("export: malloc failed\n", 2), 1);
	new_node->variable = ftt_strdup(var, ms);
	if (value)
		new_node->content = ftt_strdup(value, ms);
	else
		new_node->content = NULL;
	if (value != NULL)
	{
		new_node->full_env = ftt_strjoin(ms, var, "=");
		new_node->full_env = ftt_strjoin(ms, new_node->full_env, value);
	}
	else
		new_node->full_env = ftt_strdup(var, ms);
	new_node->next = ms->envirement;
	ms->envirement = new_node;
	return (0);
}

char	*fetch_env(char *variable, t_shell *ms)
{
	t_env	*tmp;

	tmp = ms->envirement;
	while (tmp)
	{
		if (ft_strcmp(tmp->variable, variable) == 0)
			return (tmp->content);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_target_dir(char **args, t_shell *ms)
{
	char	*target_dir;

	if (args[1] && ft_strcmp(args[1], "-") == 0)
	{
		target_dir = fetch_env("OLDPWD", ms);
		if (!target_dir)
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2), NULL);
	}
	else if (!args[1])
	{
		target_dir = fetch_env("HOME", ms);
		if (!target_dir)
			return (ft_putstr_fd("cd: HOME not set\n", 2), NULL);
	}
	else
		target_dir = args[1];
	return (target_dir);
}
