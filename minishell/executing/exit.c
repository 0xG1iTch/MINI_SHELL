/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 07:08:51 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/27 20:16:47 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	calculate_exit_code(const char *str, int *exit_code)
{
	char	*endptr;
	long	result;

	errno = 0;
	result = ft_strtol(str, &endptr);
	if (errno == ERANGE || *endptr != '\0' || endptr == str)
		return (0);
	*exit_code = (int)(result % 256);
	if (result < 0)
		*exit_code = (256 + *exit_code) % 256;
	return (1);
}

void	wrong_number(t_shell *ms, char **args, int exit_num)
{
	char	*er_msg;

	er_msg = ftt_strjoin(ms, args[1], ": numeric argument required");
	if (!er_msg)
	{
		cleanup_heredoc_files(ms);
		free_and_flee(ms, 2);
	}
	cmd_err(ms, "exit", er_msg, 2);
	cleanup_heredoc_files(ms);
	free_and_flee(ms, exit_num);
}

void	destroy(t_shell *ms, int code)
{
	cleanup_heredoc_files(ms);
	free_and_flee(ms, code);
}

int	exit_(t_shell *ms)
{
	char	**args;
	int		exit_code;

	if (!ms || !ms->executing)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit(ms->exit_status % 256);
	}
	args = get_exit_args(ms);
	if (count_nodes(ms->executing) == 1)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!args[1])
		destroy(ms, ms->exit_status % 256);
	if (!is_numeric(args[1]) || !calculate_exit_code(args[1], &exit_code))
		wrong_number(ms, args, 2);
	if (args[2])
		return (cmd_err(ms, "exit", "too many arguments", 1));
	destroy(ms, exit_code);
	return (0);
}
