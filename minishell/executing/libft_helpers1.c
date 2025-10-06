/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_helpers1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 09:39:49 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/17 18:44:18 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] == s2[i]) && s1[i] && s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*ftt_strdup(const char *s1, t_shell *ms)
{
	char	*dest;
	int		j;

	j = 0;
	dest = ft_alloc(ft_strlen(s1) + 1, &ms->collector, 0);
	if (!dest)
		return (NULL);
	while (s1[j])
	{
		dest[j] = s1[j];
		j++;
	}
	dest[j] = '\0';
	return (dest);
}

void	close_unused_fds(t_exec_list *cmd)
{
	if (cmd->in != STDIN_FILENO)
		close(cmd->in);
	if (cmd->out != STDOUT_FILENO)
		close(cmd->out);
}

void	cleanup_heredoc_files(t_shell *shell)
{
	int	i;

	if (!shell->heredoc_files)
		return ;
	i = 0;
	while (shell->heredoc_files[i])
	{
		unlink(shell->heredoc_files[i]);
		i++;
	}
	shell->heredoc_files = NULL;
}
