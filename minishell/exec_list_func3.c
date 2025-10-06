/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_list_func3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:39:26 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/21 13:53:57 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_heredoc_limit(t_shell *mini)
{
	int			count;
	t_exec_list	*exec;
	t_redir		*redir;

	exec = mini->executing;
	count = 0;
	while (exec)
	{
		redir = exec->redir_list;
		count = 0;
		while (redir)
		{
			if (redir->type == 4)
				count++;
			redir = redir->next;
		}
		if (count >= 16)
		{
			printf("minishell: maximum here-document count exceeded\n");
			mini->exit_status = 2;
			free_and_flee(mini, 2);
		}
		exec = exec->next;
	}
}
