/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:02:02 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 19:25:12 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	updating_helper(t_shell *ms, t_token *curr, char **words, int i)
{
	t_token			*new;
	static t_token	*last;

	new = ft_alloc(sizeof(t_token), &ms->collector, 0);
	if (!new)
		free_and_flee(ms, 1);
	new->cmd = ft_alloc(ft_strlen(words[i]) + 1, &ms->collector, 0);
	if (!new->cmd)
		free_and_flee(ms, 1);
	ft_strlcpy(new->cmd, words[i], ft_strlen(words[i]) + 1);
	if (i == 1)
		last = NULL;
	if (!last)
	{
		new->next = curr->next;
		curr->next = new;
	}
	else
	{
		new->next = last->next;
		last->next = new;
	}
	last = new;
}

void	updating_token_list(t_shell *ms, t_token *searched_node, char **words)
{
	t_token	*curr;
	int		nodes_num;
	int		i;

	i = 0;
	curr = ms->token;
	while (curr && curr != searched_node)
		curr = curr->next;
	nodes_num = count_words2(curr->cmd) - 1;
	while (i < nodes_num + 1)
	{
		if (i == 0)
		{
			curr->cmd = NULL;
			curr->cmd = words[i];
		}
		else
			updating_helper(ms, curr, words, i);
		i++;
	}
}

t_token	*reseting_splited_flags(t_shell *ms, t_token *curr, int word_count)
{
	int		i;
	int		j;
	int		count;
	t_token	*first_cmd;

	1 && (i = 0, first_cmd = curr, curr = curr->next);
	count = ft_strlen(first_cmd->cmd) + word_count - 1;
	while (i < word_count)
	{
		j = 0;
		if (curr)
		{
			curr->quotes_flag = NULL;
			setting_flag(ms, curr);
			while (curr->quotes_flag[j])
			{
				curr->quotes_flag[j] = first_cmd->quotes_flag[count + j];
				j++;
			}
			count += ft_strlen(curr->cmd);
			curr = curr->next;
		}
		i++;
	}
	return (setting_flag(ms, first_cmd), curr);
}

t_token	*expanding_spliting(t_shell *ms, t_token *curr)
{
	char	**new_word;
	int		word_count;

	word_count = count_words2(curr->cmd);
	new_word = ft_alloc(word_count + 1, &ms->collector, 0);
	if (!new_word)
		free_and_flee(ms, 1);
	new_word = ftt_split_expand(ms, curr->cmd);
	updating_token_list(ms, curr, new_word);
	curr = reseting_splited_flags(ms, curr, word_count);
	return (curr);
}

int	expanding_input(t_shell *shell)
{
	t_token	*curr;

	curr = shell->token;
	while (curr)
	{
		curr->quotes_flag = NULL;
		setting_flag(shell, curr);
		curr = curr->next;
	}
	if (check_expand(shell) == 0)
	{
		if (expanding_input_helper(shell) == 1)
			return (1);
	}
	strip_out_quotes(shell);
	if (command_list(shell) == 0)
	{
		reset_env(shell);
		shell->exit_status = exec_(shell);
	}
	return (0);
}
