/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:22:38 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/15 12:19:35 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signals_ignore(t_shell *ms)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{
		printf("signals error\n");
		free_and_flee(ms, 1);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		printf("signals error\n");
		free_and_flee(ms, 1);
	}
}

void	sigint_redir2(int sig)
{
	(void)sig;
	printf("\n");
	exit (SIGINT);
}

void	another_shell_signals(t_shell *ms)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
	{
		printf("signals error\n");
		free_and_flee(ms, 1);
	}
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
	{
		printf("signals error\n");
		free_and_flee(ms, 1);
	}
}

void	heredoc_signals(t_shell *ms)
{
	if (signal(SIGINT, sigint_redir2) == SIG_ERR)
	{
		printf("signals error\n");
		free_and_flee(ms, 1);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		printf("signals error\n");
		free_and_flee(ms, 1);
	}
}

void	signals_exit_status(int ac, t_shell *ms)
{
	static t_shell	*shell_copy;

	if (ac == 0)
		shell_copy = ms;
	else
		shell_copy->exit_status = ac + 128;
}
