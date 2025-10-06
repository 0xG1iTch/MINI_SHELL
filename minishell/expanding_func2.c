/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_func2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:03:46 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/16 19:09:02 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(t_shell *ms, char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*f;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_alloc(sizeof(char), &ms->collector, 0));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	f = ft_alloc(len + 1, &ms->collector, 0);
	if (f == NULL)
		return (NULL);
	while (i < len)
	{
		f[i] = s[start + i];
		i++;
	}
	f[len] = '\0';
	return (f);
}

void	exit_status_case(t_shell *shell, t_token *curr, int n)
{
	char	*str_n;

	str_n = ft_itoa(n);
	curr->cmd = ftt_strjoin(shell, curr->cmd, str_n);
	free(str_n);
}

static char	*helper(t_shell *shell, t_token *curr, char *temp, int var_start)
{
	if (var_start - 1 > 0)
		temp = ft_substr(shell, curr->cmd, 0, var_start - 1);
	else
	{
		temp = ft_alloc(1, &shell->collector, 0);
		temp[0] = '\0';
	}
	return (temp);
}

static char	*helper2(t_shell *shell, t_token *curr, size_t *i, int *var_len)
{
	char	*temp;
	int		var_start;

	temp = NULL;
	var_start = (*i);
	while (curr->cmd[*i] && is_valid(curr->cmd[*i], 1))
		(*i)++;
	*var_len = *i - var_start;
	return (helper(shell, curr, temp, var_start));
}

int	normal_expanding(size_t *i, t_token *curr, t_shell *shell)
{
	int		var_start;
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		var_len;

	var_start = *i;
	if (curr->quotes_flag[*i] == 'e')
		return (*i);
	temp = helper2(shell, curr, i, &var_len);
	var_name = ft_strndup(curr->cmd + var_start, var_len);
	if (ft_strcmp(var_name, "?") == 0)
		var_value = ftt_itoa(shell, shell->exit_status);
	else
		var_value = get_env_value(shell, var_name);
	curr->got_expanded = ft_strlen(var_value);
	if (var_value && var_value[0] != '\0' && temp)
	{
		var_value = ftt_strjoin(shell, var_value, curr->cmd + (*i));
		curr->cmd = ftt_strjoin(shell, temp, var_value);
	}
	else
		1 && (curr->cmd = ftt_strjoin(shell, temp, curr->cmd + (*i)), (*i)--);
	*i = (*i) - var_len;
	return (free(var_name), var_name = NULL, curr->got_expanded);
}
