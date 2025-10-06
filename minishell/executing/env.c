/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:38:32 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/16 17:10:06 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	closing(int first, int second)
{
	close(first);
	close(second);
}

int	dup_for_built_in(t_shell *ms)
{
	int			stdin_save;
	int			stdout_save;
	int			ret;
	t_exec_list	*executing;

	executing = ms->executing;
	ret = 0;
	stdin_save = dup(STDIN_FILENO);
	stdout_save = dup(STDOUT_FILENO);
	if (executing->in != STDIN_FILENO)
		dup2(executing->in, STDIN_FILENO);
	if (executing->out != STDOUT_FILENO)
		dup2(executing->out, STDOUT_FILENO);
	if (executing->in != STDIN_FILENO)
		close(executing->in);
	if (executing->out != STDOUT_FILENO)
		close(executing->out);
	ret = execute_cmd(executing, ms);
	dup2(stdin_save, STDIN_FILENO);
	dup2(stdout_save, STDOUT_FILENO);
	closing(stdin_save, stdout_save);
	return (ret);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (
		ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0
	);
}

int	env_(t_shell *minishell)
{
	char	**args;
	t_env	*current;

	args = get_env_args(minishell);
	if (!args[1])
	{
		current = minishell->envirement;
		while (current)
		{
			if (current->content)
				printf("%s=%s\n", current->variable, current->content);
			current = current->next;
		}
		return (0);
	}
	return (0);
}
