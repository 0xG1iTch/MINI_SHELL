/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:38:35 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/04/27 19:53:18 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	unset_env(char *var_name, t_shell *minishell)
{
	t_env	*prev;
	t_env	*current;

	current = minishell->envirement;
	prev = NULL;
	while (current != NULL)
	{
		if (ft_strcmp(current->variable, var_name) == 0)
		{
			if (prev == NULL)
				minishell->envirement = current->next;
			else
				prev->next = current->next;
		}
		prev = current;
		current = current->next;
	}
}

int	is_valid_identifier(char *name)
{
	if (!name || !*name || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	while (*++name)
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
	return (1);
}

int	unset_(t_shell *minishell)
{
	char	**args;
	int		status;
	int		i;

	args = minishell->executing->args;
	status = 0;
	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: '", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			status = 1;
		}
		unset_env(args[i], minishell);
		i++;
	}
	return (status);
}
