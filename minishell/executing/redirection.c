/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:05:00 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/27 20:09:15 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_input_redir(t_shell *ms, t_redir *redir, t_exec_list *cmd)
{
	int	fd;
	int	status;

	status = 0;
	if (cmd->in != STDIN_FILENO)
		close(cmd->in);
	if (redir->type == HEREDOC)
	{
		fd = handle_heredoc(ms, redir->file, !redir->do_not_expand, status);
		if (fd < 0)
			return (130);
	}
	else
	{
		fd = open(redir->file, O_RDONLY);
		if (fd == -1)
			return (cmd_err(ms, redir->file, strerror(errno), errno), 1);
	}
	cmd->in = fd;
	return (0);
}

static int	handle_output_redir(t_shell *ms, t_redir *redir, t_exec_list *cmd)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (redir->type == OUT_REDIR)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	if (cmd->out != STDOUT_FILENO)
		close(cmd->out);
	fd = open(redir->file, flags, 0644);
	if (fd == -1)
		return (cmd_err(ms, redir->file, strerror(errno), errno), 1);
	cmd->out = fd;
	return (0);
}

int	handle_redirections(t_shell *ms, t_exec_list *cmd)
{
	t_redir	*current;
	int		status;

	cmd->in = STDIN_FILENO;
	cmd->out = STDOUT_FILENO;
	current = cmd->redir_list;
	while (current)
	{
		if (current->type == IN_REDIR || current->type == HEREDOC)
		{
			status = handle_input_redir(ms, current, cmd);
			if (status != 0)
				return (status);
		}
		else if (current->type == OUT_REDIR || current->type == APPEND_REDIR)
		{
			if (handle_output_redir(ms, current, cmd) != 0)
				return (1);
		}
		current = current->next;
	}
	return (0);
}
