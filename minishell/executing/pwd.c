/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:45:44 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/17 17:34:31 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd_(t_shell *minishell)
{
	char	*pwd_env;
	char	cwd[PATH_MAX];

	pwd_env = fetch_env("PWD", minishell);
	if (pwd_env != NULL)
	{
		ft_putendl_fd(pwd_env, STDOUT_FILENO);
		return (0);
	}
	if (getcwd(cwd, PATH_MAX) != NULL)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (0);
	}
	perror("pwd");
	return (1);
}
