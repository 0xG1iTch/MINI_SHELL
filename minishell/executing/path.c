/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-haya <yel-haya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 22:34:02 by mel-hamm          #+#    #+#             */
/*   Updated: 2025/05/21 18:11:19 by yel-haya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*full_path(char *cmd, t_shell *ms)
{
	if (ft_strchr(cmd, '/') != NULL)
		return (ftt_strdup(cmd, ms));
	return (NULL);
}

char	*current_dir_path(char *cmd, t_shell *ms)
{
	char	*temp;
	char	*temp1;
	char	*temp2;
	char	cwd[PATH_MAX];

	temp = ft_strchr(cmd, '/');
	temp2 = ft_strchr(cmd, '.');
	if (temp != NULL && temp2 && temp2[0] == '.')
	{
		getcwd(cwd, PATH_MAX);
		temp1 = ft_strtrim(cmd, ".");
		cmd = ftt_strjoin(ms, cwd, temp1);
		free(temp1);
		return (cmd);
	}
	return (NULL);
}

char	*check__construct_path(char **paths, char *cmd, t_shell *ms)
{
	int		i;
	char	*half_path;
	char	*path;

	i = -1;
	while (paths[++i])
	{
		if (paths[i][0] == '\0')
			continue ;
		half_path = ftt_strjoin(ms, paths[i], "/");
		if (half_path)
			path = ftt_strjoin(ms, half_path, cmd);
		if (access(path, X_OK) == 0)
			return (path);
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **env, t_shell *ms)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	path = current_dir_path(cmd, ms);
	if (!path)
		path = full_path(cmd, ms);
	if (path)
		return (path);
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i])
	{
		paths = ftt_split(ms, env[i] + 5, ':');
		if (!paths)
			return (NULL);
		path = check__construct_path(paths, cmd, ms);
		if (path)
			return (path);
	}
	return (NULL);
}
