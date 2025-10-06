/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_func5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:55:44 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 20:02:46 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*splitting_check(t_shell *shell, t_token *current)
{
	if (count_words2(current->cmd) > 1 && \
	!inside_quotes(current->cmd) \
	&& current->op != 1005 && current->op != 1006 && current->op != 1007 \
	&& current->op != 1008)
	{
		if (current->prec && current->prec->cmd && \
			ft_strcmp(current->prec->cmd, "echo") == 0)
			return (expanding_spliting(shell, current));
		if (current->prec && (current->prec->cmd && \
			ft_strcmp(current->prec->cmd, "export") == 0) \
		&& current->quotes_flag[0] != 'e' && \
		current->cmd[0] != '=' && current->cmd[0] != '"')
			return (current);
		else if (current->cmd[0] == '$')
			return (current);
		else if (current->cmd[0] == '=')
			return (expanding_spliting(shell, current));
		else
			return (expanding_spliting(shell, current));
	}
	else if ((current->op == 1005 || current->op == 1006 || current->op == 1007 \
		|| current->op == 1008) && current->cmd && current->cmd[0] != '$' \
		&& count_words2(current->cmd) > 1 && !inside_quotes(current->cmd))
		return (expanding_spliting(shell, current));
	return (current);
}

void	expanding_quotes_cases(t_token *c, int *in_s, int *in_d, size_t i)
{
	if (c->cmd[i] == '"' && !(*in_s) && c->quotes_flag[i] == 'c')
		*in_d = !(*in_d);
	else if (c->cmd[i] == '\'' && !(*in_d) && c->quotes_flag[i] == 'c')
		*in_s = !(*in_s);
}

void	reset_env_helper(t_shell *minishell, int word_len, int k)
{
	int		i;
	int		c;
	t_env	*courant;
	char	**env;

	i = 0;
	c = 0;
	env = ft_alloc((sizeof(char *) * (k + 1)), &minishell->collector, 0);
	if (!env)
		free_and_flee(minishell, 1);
	courant = minishell->envirement;
	while (courant)
	{
		word_len = ft_strlen(courant->full_env);
		env[i] = ft_alloc(word_len + 1, &minishell->collector, 0);
		if (!env[i])
			free_and_flee(minishell, 1);
		ft_strlcpy(env[i], courant->full_env, word_len + 1);
		i++;
		courant = courant->next;
		c = c + word_len;
	}
	env[i] = NULL;
	minishell->env = env;
}

int	expand_work(t_token *curr, t_shell *shell)
{
	char	*old_str;

	old_str = malloc(ft_strlen(curr->cmd) + 1);
	if (!old_str)
		free_and_flee(shell, 1);
	ft_memcpy(old_str, curr->cmd, ft_strlen(curr->cmd) + 1);
	expanding_helper(shell, curr);
	free(old_str);
	return (0);
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*dest;
	size_t	len;
	size_t	j;

	len = ft_strlen(s1);
	if (len > n)
		len = n;
	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	j = 0;
	while (j < len && s1[j])
	{
		dest[j] = s1[j];
		j++;
	}
	dest[j] = '\0';
	return (dest);
}
