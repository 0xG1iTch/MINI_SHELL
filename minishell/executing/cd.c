/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:55:15 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/24 18:50:57 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	update_env_variables(char *old_pwd, char *new_pwd, t_shell *ms)
{
	if (old_pwd)
	{
		if (set_env("OLDPWD", old_pwd, ms) == 1)
			return (ft_putstr_fd("cd: failed to update OLDPWD\n", 2), 1);
	}
	if (set_env("PWD", new_pwd, ms) == 1)
		return (ft_putstr_fd("cd: failed to update PWD\n", 2), 1);
	return (0);
}

char	*calculate_logical_path(char *old_pwd, char *target_dir, t_shell *ms)
{
	char	*new_pwd;
	char	*last_slash;

	if (!old_pwd)
		old_pwd = "";
	if (target_dir[0] == '/')
		new_pwd = ftt_strdup(target_dir, ms);
	else if (ft_strcmp(target_dir, "..") == 0)
	{
		new_pwd = ftt_strdup(old_pwd, ms);
		last_slash = ft_strrchr(new_pwd, '/');
		if (last_slash == new_pwd)
			new_pwd[1] = '\0';
		else
			*last_slash = '\0';
	}
	else if (ft_strcmp(target_dir, ".") == 0)
		new_pwd = ftt_strdup(old_pwd, ms);
	else
	{
		new_pwd = ftt_strjoin(ms, old_pwd, "/");
		new_pwd = ftt_strjoin(ms, new_pwd, target_dir);
	}
	return (new_pwd);
}

void	handle_cd_dash(char **args, t_shell *ms)
{
	char	*current_pwd;

	if (args[1] && ft_strcmp(args[1], "-") == 0)
	{
		current_pwd = fetch_env("PWD", ms);
		if (current_pwd)
			printf("%s\n", current_pwd);
	}
}

int	ft_chdir(char *trgt_dir, char *old_pwd, t_shell *ms)
{
	char	*new_pwd;
	char	cwd_buffer[PATH_MAX];

	if (chdir(trgt_dir) != 0)
		return (cmd_err(ms, ftt_strjoin(ms, "cd: ", trgt_dir), \
		strerror(errno), 1));
	if (getcwd(cwd_buffer, PATH_MAX) == NULL && errno == ENOENT)
	{
		if (ft_strcmp(trgt_dir, "..") == 0)
			ft_putendl_fd("error retrieving current directory: getcwd: \
cannot access parent directories: No such file or directory", 2);
	}
	if (getcwd(cwd_buffer, PATH_MAX) != NULL)
	{
		if (update_env_variables(old_pwd, cwd_buffer, ms) != 0)
			return (1);
	}
	else
	{
		new_pwd = calculate_logical_path(old_pwd, trgt_dir, ms);
		if (update_env_variables(old_pwd, new_pwd, ms) != 0)
			return (1);
	}
	return (0);
}

int	cd_(t_shell *ms)
{
	char	*target_dir;
	char	*old_pwd;
	char	**args;
	int		ret;

	args = get_cd_args(ms);
	target_dir = get_target_dir(args, ms);
	if (!target_dir)
		return (1);
	old_pwd = fetch_env("PWD", ms);
	ret = ft_chdir(target_dir, old_pwd, ms);
	if (ret != 0)
		return (ret);
	handle_cd_dash(args, ms);
	return (0);
}
