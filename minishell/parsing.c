/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:32:45 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 19:56:38 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_env(t_shell *minishell)
{
	t_env	*courant;
	int		word_len;
	int		i;

	i = 0;
	word_len = 0;
	courant = minishell->envirement;
	while (courant)
	{
		i++;
		courant = courant->next;
	}
	reset_env_helper(minishell, word_len, i);
}

void	get_line(t_shell *minishell)
{
	signals_exit_status(0, minishell);
	while (69)
	{
		main_signals(minishell);
		minishell->command = readline("---> MiniShell$ ");
		if (!minishell->command)
		{
			minishell->executing = NULL;
			exit_(minishell);
		}
		if (tokenize_input(minishell->command, minishell) == 0)
		{
			if (check_syntax(minishell) == 0 && minishell->token != NULL)
				expanding_input(minishell);
		}
		minishell->token = NULL;
		if (minishell->command[0])
			add_history(minishell->command);
		free(minishell->command);
	}
}

void	no_env(t_shell *ms)
{
	t_env	*new;
	char	pwd[PATH_MAX];

	if (getcwd(pwd, PATH_MAX) != NULL)
		new = new_lst_no_env(ms, "PWD", pwd);
	else
		new = new_lst_no_env(ms, "PWD", "(NULL)");
	ms->envirement = new;
	new = new_lst_no_env(ms, "PATH", \
	"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	if (new)
		ms->envirement->next = new;
}

void	get_env(t_shell *minishell, char **env)
{
	int		i;
	t_env	*new;
	char	**test;
	t_env	*courant;

	i = 0;
	while (env[i])
	{
		test = ftt_split(minishell, env[i], '=');
		if (!test)
			free_and_flee(minishell, 1);
		new = new_lst(minishell, env[i], test[0], test[1]);
		if (minishell->envirement == NULL)
			minishell->envirement = new;
		else
		{
			courant = minishell->envirement;
			while (courant->next)
				courant = courant->next;
			courant->next = new;
		}
		i++;
	}
	if (minishell->envirement == NULL)
		no_env(minishell);
}

void	parsing(char **env)
{
	t_shell	*minishell;

	minishell = (t_shell *)malloc(sizeof(t_shell));
	if (!minishell)
		exit(1);
	minishell->exit_status = 0;
	minishell->envirement = NULL;
	minishell->token = NULL;
	minishell->executing = NULL;
	minishell->heredoc_files = NULL;
	minishell->collector = malloc(sizeof(t_gc));
	if (!minishell->collector)
	{
		free(minishell);
		return ;
	}
	minishell->collector->ptr = NULL;
	minishell->collector->next = NULL;
	get_env(minishell, env);
	get_line(minishell);
	minishell->collector = ft_alloc(0, &minishell->collector, 1);
	free(minishell->collector);
	rl_clear_history();
	free(minishell);
}
