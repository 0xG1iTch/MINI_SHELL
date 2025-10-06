/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:49:13 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/27 17:00:11 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**get_echo_args(t_shell *ms)
{
	char		**args;
	t_exec_list	*exec;

	exec = ms->executing;
	while (exec)
	{
		if (ft_strcmp(exec->args[0], "echo") == 0 && exec->flag == 0)
		{
			args = exec->args;
			return (args);
		}
		exec = exec->next;
	}
	return (NULL);
}

char	**get_export_args(t_shell *ms)
{
	char		**args;
	t_exec_list	*exec;

	exec = ms->executing;
	while (exec)
	{
		if (ft_strcmp(exec->args[0], "export") == 0 && exec->flag == 0)
		{
			args = exec->args;
			return (args);
		}
		exec = exec->next;
	}
	return (NULL);
}

char	**get_cd_args(t_shell *ms)
{
	char		**args;
	t_exec_list	*exec;

	exec = ms->executing;
	while (exec)
	{
		if (ft_strcmp(exec->args[0], "cd") == 0 && exec->flag == 0)
		{
			args = exec->args;
			return (args);
		}
		exec = exec->next;
	}
	return (NULL);
}

char	**get_env_args(t_shell *ms)
{
	char		**args;
	t_exec_list	*exec;

	exec = ms->executing;
	while (exec)
	{
		if (ft_strcmp(exec->args[0], "env") == 0 && exec->flag == 0)
		{
			args = exec->args;
			return (args);
		}
		exec = exec->next;
	}
	return (NULL);
}

char	**get_exit_args(t_shell *ms)
{
	char		**args;
	t_exec_list	*exec;

	exec = ms->executing;
	while (exec)
	{
		if (ft_strcmp(exec->args[0], "exit") == 0 && exec->flag == 0)
		{
			args = exec->args;
			return (args);
		}
		exec = exec->next;
	}
	return (NULL);
}
