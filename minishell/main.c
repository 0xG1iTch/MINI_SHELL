/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:38:47 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 18:30:35 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	numlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
	{
		len++;
		n = -n;
	}
	while (n > 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ftt_itoa(t_shell *shell, int n)
{
	char	*str;
	int		len;

	if (n == -2147483648)
		return (ftt_strdup("-2147483648", shell));
	if (n == 0)
		return (ftt_strdup("0", shell));
	len = numlen(n);
	str = ft_alloc(len + 1, &shell->collector, 0);
	if (str == NULL)
		free_and_flee(shell, 1);
	str[len] = '\0';
	if (n < 0)
	{
		n = -n;
		str[0] = '-';
	}
	while (n > 0)
	{
		str[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}

void	main_sigint_func(int sig)
{
	(void)sig;
	signals_exit_status(SIGINT, NULL);
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	main_signals(t_shell *ms)
{
	if (signal(SIGINT, main_sigint_func) == SIG_ERR)
	{
		printf("signals error\n");
		free_and_flee(ms, 1);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		printf("signals error\n");
		free_and_flee(ms, 1);
	}
}

int	main(int ac, char **av, char **env)
{
	if (ac == 1)
	{
		(void)av;
		if (isatty(STDIN_FILENO))
			parsing(env);
		else
			printf("\033[0;31m\tSEGFAULT??\033[0m\n    NAAAAAH the input is \
not from the terminal\n");
	}
	else
		printf("Usage: ./minishell\n");
}
