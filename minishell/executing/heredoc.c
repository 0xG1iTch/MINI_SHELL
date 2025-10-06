/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:56:09 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/24 18:53:39 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_heredoc_file(t_shell *ms, char *filename)
{
	int		i;
	char	**new_files;

	if (!ms->heredoc_files)
	{
		ms->heredoc_files = ft_alloc(sizeof(char *) * 2, &ms->collector, 0);
		if (!ms->heredoc_files)
			free_and_flee(ms, 1);
		1 && (ms->heredoc_files[0] = filename, ms->heredoc_files[1] = NULL);
		return ;
	}
	i = 0;
	while (ms->heredoc_files[i] != NULL)
		i++;
	new_files = ft_alloc(sizeof(char *) * (i + 2), &ms->collector, 0);
	if (!new_files)
		free_and_flee(ms, 1);
	i = 0;
	while (ms->heredoc_files[i])
	{
		new_files[i] = ms->heredoc_files[i];
		i++;
	}
	1 && (new_files[i] = filename, new_files[i + 1] = NULL);
	ms->heredoc_files = new_files;
}

char	*generate_heredoc_filename(t_shell *ms)
{
	static int	counter = 0;
	char		*filename;
	char		*counter_str;

	counter_str = ft_itoa(counter++);
	filename = ftt_strjoin(ms, "/tmp/heredoc_", counter_str);
	free(counter_str);
	while (access(filename, F_OK) == 0)
	{
		filename = NULL;
		counter_str = ft_itoa(counter++);
		filename = ftt_strjoin(ms, "/tmp/heredoc_", counter_str);
		if (!filename)
			return (NULL);
		free(counter_str);
	}
	return (filename);
}

void	read_heredoc_input(t_shell *ms, char *delimiter, int fd, int expnd_flg)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		if (expnd_flg == 0)
			line = expanding_heredoc(ms, line);
		ft_putendl_fd(line, fd);
		free(line);
	}
	if (line)
		free(line);
	close(fd);
}

void	heredoc_child(int *fd, t_shell *ms, char *delimiter, int expand_flag)
{
	close(fd[1]);
	heredoc_signals(ms);
	read_heredoc_input(ms, delimiter, fd[0], expand_flag);
	close (fd[0]);
	exit(0);
}

int	handle_heredoc(t_shell *ms, char *delimiter, int expand_flag, int status)
{
	char	*filename;
	int		fd[2];
	pid_t	pid;

	filename = generate_heredoc_filename(ms);
	fd[0] = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd[0] == -1)
		return (cmd_err(ms, "open", strerror(errno), errno));
	1 && (add_heredoc_file(ms, filename), fd[1] = open(filename, O_RDONLY));
	if (fd[1] == -1)
		return (close(fd[0]), cmd_err(ms, "open", strerror(errno), errno));
	pid = fork();
	if (pid == -1)
		return (closing(fd[0], fd[1]), cmd_err(ms, \
			"fork", strerror(errno), errno));
	if (pid == 0)
		heredoc_child(fd, ms, delimiter, expand_flag);
	close(fd[0]);
	signals_ignore(ms);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		return (close(fd[1]), -1);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (close(fd[1]), -1);
	return (fd[1]);
}
