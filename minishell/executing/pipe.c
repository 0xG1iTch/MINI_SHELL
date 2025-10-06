/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:14:53 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/27 20:23:16 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	wait_children(t_shell *ms, int pid)
{
	int	status;
	int	x;

	status = 0;
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid failed");
		free_and_flee(ms, 1);
	}
	while (wait(&x) != -1)
		;
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ms->exit_status = 128 + WTERMSIG(status);
	return (ms->exit_status);
}

void	close_pipes(int p_fd[2], t_exec_list *next)
{
	if (next)
	{
		close(p_fd[0]);
		close(p_fd[1]);
	}
}

void	child_process(t_exec_list *cmd, int p_fd[2], int prev_in, t_shell *ms)
{
	another_shell_signals(ms);
	if (prev_in != STDIN_FILENO)
	{
		if (dup2(prev_in, STDIN_FILENO) == -1)
			cmd_err(ms, "dup2: ", strerror(errno), errno);
		close(prev_in);
	}
	if (cmd->next)
	{
		if (dup2(p_fd[1], STDOUT_FILENO) == -1)
			cmd_err(ms, "dup2: ", strerror(errno), errno);
		close(p_fd[1]);
	}
	close_pipes(p_fd, cmd->next);
	if (cmd->in != STDIN_FILENO)
	{
		if (dup2(cmd->in, STDIN_FILENO) == -1)
			cmd_err(ms, "dup2: ", strerror(errno), errno);
	}
	if (cmd->out != STDOUT_FILENO)
	{
		if (dup2(cmd->out, STDOUT_FILENO) == -1)
			cmd_err(ms, "dup2: ", strerror(errno), errno);
	}
	exit(execute_cmd(cmd, ms));
}

static void	parent_cleanup(int *prev_in, int p_fd[2], t_exec_list *current)
{
	close_unused_fds(current);
	if (*prev_in != STDIN_FILENO)
		close(*prev_in);
	if (current->next)
		*prev_in = p_fd[0];
	else
		*prev_in = STDIN_FILENO;
	close(p_fd[1]);
}

int	execute_pipeline(t_shell *ms)
{
	int			p_fd[2];
	pid_t		pid;
	t_exec_list	*current;
	int			prev_pipe_in;
	int			status;

	current = ms->executing;
	prev_pipe_in = STDIN_FILENO;
	while (current)
	{
		status = handle_redirections(ms, current);
		if (status != 0)
			check_failure(status, p_fd, current, ms);
		if (current->next && pipe(p_fd) == -1)
			return (cmd_err(ms, "pipe", strerror(errno), errno), 1);
		pid = fork();
		if (pid == -1)
			return (cmd_err(ms, "fork", strerror(errno), errno), 1);
		if (pid == 0)
			child_process(current, p_fd, prev_pipe_in, ms);
		signals_ignore(ms);
		parent_cleanup(&prev_pipe_in, p_fd, current);
		1 && (current->flag = 1, current = current->next);
	}
	return (wait_children(ms, pid));
}
