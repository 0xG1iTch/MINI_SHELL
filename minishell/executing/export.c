/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:39:45 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/15 16:03:23 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_string_array(char **arr, int size)
{
	int		i;
	int		swapped;
	char	*temp;

	if (!arr || size < 2)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < size - 1)
		{
			if (ft_strcmp(arr[i], arr[i + 1]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
				swapped = 1;
			}
			i++;
		}
		size--;
	}
}

static void	print_env_var(char *env_str)
{
	char	*equal_sign;
	char	*key;

	if (!env_str)
		return ;
	equal_sign = ft_strchr(env_str, '=');
	if (equal_sign)
	{
		key = ft_strndup(env_str, equal_sign - env_str);
		if (key)
		{
			printf("declare -x %s=\"%s\"\n", key, equal_sign + 1);
			free(key);
		}
	}
	else
		printf("declare -x %s\n", env_str);
}

static void	print_sorted_env(char **env_arr, int count)
{
	int	i;

	sort_string_array(env_arr, count);
	i = 0;
	while (i < count)
	{
		if (env_arr[i] != NULL)
			print_env_var(env_arr[i]);
		i++;
	}
}

static void	setup_exported_vars(t_shell *ms)
{
	t_env	*current;
	char	**env_arr;
	int		count;

	count = 0;
	current = ms->envirement;
	while (current && ++count)
		current = current->next;
	if (count == 0)
		return ;
	env_arr = malloc(count * sizeof(char *));
	if (!env_arr)
		return ;
	current = ms->envirement;
	count = 0;
	while (current)
	{
		if (current->full_env)
			env_arr[count++] = current->full_env;
		current = current->next;
	}
	print_sorted_env(env_arr, count);
	free(env_arr);
}

int	export_(t_shell *ms)
{
	int		i;
	int		status;
	char	**args;

	i = 1;
	status = 0;
	args = get_export_args(ms);
	if (!args[i])
	{
		setup_exported_vars(ms);
		return (0);
	}
	while (args[i])
	{
		status |= handle_export_arg(args[i], ms);
		i++;
	}
	return (status);
}
