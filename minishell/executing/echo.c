/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 08:06:12 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/27 20:23:40 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_failure(int status, int *p_fd, t_exec_list *current, t_shell *ms)
{
	if (status == 1)
	{
		close_unused_fds(current);
		close_pipes(p_fd, current);
	}
	return (ms->exit_status = status);
}

static int	is_valid_n_option(const char *arg)
{
	size_t	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	return (i > 1 && arg[i] == '\0');
}

int	echo_(t_shell *minishell)
{
	char		**args;
	int			rm_newline;
	int			i;

	args = get_echo_args(minishell);
	rm_newline = 0;
	i = 1;
	while (args[i] && is_valid_n_option(args[i]))
	{
		rm_newline = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!rm_newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
