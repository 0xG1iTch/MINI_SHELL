/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:13:04 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/15 16:04:18 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*helper_heredoc(t_shell *shell, \
	char *line, char *temp, int var_start)
{
	if (var_start - 1 > 0)
		temp = ft_substr(shell, line, 0, var_start - 1);
	else
	{
		temp = ft_alloc(1, &shell->collector, 0);
		temp[0] = '\0';
	}
	return (temp);
}

char	*normal_expanding_heredoc(size_t *i, char *line, t_shell *shell)
{
	int		var_start;
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		var_len;

	1 && (temp = NULL, var_start = *i);
	while (line[*i] && is_valid(line[*i], 1))
		(*i)++;
	var_len = *i - var_start;
	temp = helper_heredoc(shell, line, temp, var_start);
	var_name = ft_strndup(line + var_start, var_len);
	if (ft_strcmp(var_name, "?") == 0)
		var_value = ftt_itoa(shell, shell->exit_status);
	else
		var_value = get_env_value(shell, var_name);
	if (var_value && temp)
	{
		var_value = ftt_strjoin(shell, var_value, line + (*i));
		line = ftt_strjoin(shell, temp, var_value);
	}
	else
		line = ftt_strjoin(shell, temp, var_value);
	free(var_name);
	return (var_name = NULL, line);
}

char	*expanding_helper_heredoc(t_shell *shell, char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && (line[i + 1] && is_valid(line[i + 1], 1)))
		{
			i++;
			line = normal_expanding_heredoc(&i, line, shell);
			if (ft_strlen(line) < i)
				break ;
		}
		else
			i++;
	}
	return (line);
}

int	check_expand_heredoc(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			return (0);
		i++;
	}
	return (1);
}

char	*expanding_heredoc(t_shell *shell, char *line)
{
	if (check_expand_heredoc(line) == 0)
		line = expanding_helper_heredoc(shell, line);
	return (line);
}
