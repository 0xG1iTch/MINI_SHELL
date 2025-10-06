/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:32:13 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/24 18:52:13 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_nodes(t_exec_list *exec)
{
	int	nodes;

	nodes = 0;
	while (exec)
	{
		nodes++;
		exec = exec->next;
	}
	return (nodes);
}

int	cmd_err(t_shell *ms, char *cmd, char *msg, int status)
{
	char	*err_msg;

	err_msg = NULL;
	err_msg = ftt_strjoin(ms, "minishell: ", cmd);
	err_msg = ftt_strjoin(ms, err_msg, ": ");
	err_msg = ftt_strjoin(ms, err_msg, msg);
	err_msg = ftt_strjoin(ms, err_msg, "\n");
	write(2, err_msg, ft_strlen(err_msg));
	return (status);
}

static long	parse_digits(const char *s, char **endptr)
{
	long	result;
	int		overflow;
	int		digit;

	result = 0;
	overflow = 0;
	while (*s >= '0' && *s <= '9')
	{
		digit = *s - '0';
		if (!overflow && result > (LONG_MAX - digit) / 10)
			overflow = 1;
		if (!overflow)
			result = result * 10 + digit;
		s++;
	}
	if (overflow)
		errno = ERANGE;
	if (endptr)
		*endptr = (char *)s;
	return (result);
}

long	ft_strtol(const char *str, char **endptr)
{
	const char	*s;
	int			sign;
	long		val;

	s = str;
	sign = 1;
	errno = 0;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	val = parse_digits(s, endptr);
	if (errno == ERANGE)
	{
		if (sign > 0)
			return (LONG_MAX);
		return (LONG_MIN);
	}
	return (sign * val);
}
