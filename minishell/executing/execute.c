/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:23:43 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/24 18:51:50 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_(t_shell *ms)
{
	t_exec_list	*cmd;
	int			status;

	cmd = ms->executing;
	if (ms->executing->next != NULL)
		return (execute_pipeline(ms));
	else
	{
		status = handle_redirections(ms, ms->executing);
		if (status == 130 || status == 1)
			return (status);
		if (!cmd->cmd)
			return (close_unused_fds(cmd), 0);
		if (is_builtin(cmd->cmd))
			return (dup_for_built_in(ms));
		return (execute_cmd(ms->executing, ms));
	}
	return (0);
}

int	dup_to_execv(t_shell *ms, t_exec_list *cmd, char *path)
{
	if (cmd->in != STDIN_FILENO)
	{
		if (dup2(cmd->in, STDIN_FILENO) == -1)
			return (cmd_err(ms, "dup2", strerror(errno), errno));
	}
	if (cmd->out != STDOUT_FILENO)
	{
		if (dup2(cmd->out, STDOUT_FILENO) == -1)
			return (cmd_err(ms, "dup2", strerror(errno), errno));
	}
	if (cmd->in != STDIN_FILENO)
		close(cmd->in);
	if (cmd->out != STDOUT_FILENO)
		close(cmd->out);
	if (execve(path, cmd->args, ms->env) == -1)
		return (cmd_err(ms, cmd->cmd, strerror(errno), 126));
	return (0);
}

int	sig_status(t_exec_list *cmd, int status)
{
	if (cmd->in != STDIN_FILENO)
		close(cmd->in);
	if (cmd->out != STDOUT_FILENO)
		close(cmd->out);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

int	exec_external(t_exec_list *cmd, t_shell *ms)
{
	char		*path;
	pid_t		pid;
	int			status;
	struct stat	file_stat;

	path = get_cmd_path(cmd->cmd, ms->env, ms);
	if (!path)
		return (ms->exit_status = cmd_err(ms, cmd->cmd, \
			"command not found", 127));
	if (stat(path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		return (ms->exit_status = cmd_err(ms, path, "is a directory", 126));
	if (access(path, X_OK) != 0)
		return (ms->exit_status = cmd_err(ms, cmd->cmd, strerror(errno), 126));
	pid = fork();
	if (pid == -1)
		return (ms->exit_status = cmd_err(ms, \
			"fork", strerror(errno), errno), 1);
	if (pid == 0)
	{
		1 && (another_shell_signals(ms), status = dup_to_execv(ms, cmd, path));
		exit(status);
	}
	signals_ignore(ms);
	waitpid(pid, &status, 0);
	return (ms->exit_status = sig_status(cmd, status));
}

int	execute_cmd(t_exec_list *cmd, t_shell *ms)
{
	if (!cmd->cmd || !cmd->cmd[0])
	{
		if (cmd->redir_list && cmd->redir_list->file)
			return (close_unused_fds(cmd), 0);
		if (cmd->got_expanded == 0)
			return (cmd_err(ms, "", "command not found", 127));
		else
			return (0);
	}
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		return (echo_(ms));
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		return (cd_(ms));
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		return (pwd_(ms));
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		return (export_(ms));
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		return (unset_(ms));
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		return (exit_(ms));
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		return (env_(ms));
	ms->exit_status = exec_external(cmd, ms);
	return (ms->exit_status);
}
