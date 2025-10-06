/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 00:28:58 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/24 18:33:25 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_export_arg(char *arg, t_shell *ms)
{
	t_list	*lst;

	(1) && (lst = malloc (sizeof(t_list)), lst->apnd = 0);
	if (!lst)
		return (perror("malloc"), errno);
	lst->sign = ft_strchr(arg, '=');
	if (!lst->sign)
		lst->var = ftt_strdup(arg, ms);
	else
	{
		if (lst->sign > arg && *(lst->sign - 1) == '+')
		{
			lst->apnd = 1;
			lst->var = ftt_strndup(arg, (lst->sign - arg) - 1, ms);
		}
		else
			lst->var = ftt_strndup(arg, lst->sign - arg, ms);
		lst->value = lst->sign + 1;
	}
	if (!lst->var)
		return (free(lst), 1);
	if (!is_valid_identifier(lst->var))
		return (free(lst), cmd_err(ms, ftt_strjoin(ms, "export: '", \
ftt_strjoin(ms, arg, "'")), "not a valid identifier", 1));
	return (assign_or_append(lst, lst->apnd, ms));
}

static int	handle_appnd_value(char *var_name, char *var_value, t_shell *ms)
{
	char	*current_value;
	char	*new_value;
	int		ret;

	current_value = fetch_env(var_name, ms);
	if (current_value)
	{
		new_value = ft_strjoin(current_value, var_value);
		if (!new_value)
			return (1);
		ret = set_env(var_name, new_value, ms);
		free(new_value);
	}
	else
		ret = set_env(var_name, var_value, ms);
	return (ret);
}

int	assign_or_append(t_list	*lst, int appnd, t_shell *ms)
{
	int	ret;

	ret = 0;
	if (lst->sign)
	{
		if (appnd)
			ret = handle_appnd_value(lst->var, lst->value, ms);
		else
			ret = set_env(lst->var, lst->value, ms);
	}
	else if (!fetch_env(lst->var, ms))
		ret = set_env(lst->var, NULL, ms);
	free(lst);
	return (ret);
}
