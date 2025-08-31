/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.s19.be>         #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-25 16:54:10 by elara-va          #+#    #+#             */
/*   Updated: 2025-08-25 16:54:10 by elara-va         ###   ########.be       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	run_abs_path_cmd(char **full_cmd, t_files files_info,
	char *envp[])
{
	if (execve(full_cmd[0], full_cmd, envp) == -1)
	{
		managerr(2, full_cmd[0], files_info);
		free_str_arr(full_cmd);
		exit(EXIT_FAILURE);
	}
	return ;
}

static int	look_for_valid_path(char **possible_paths, t_cmd *cmd_info)
{
	size_t	i;
	char	*absolute_path;

	i = 0;
	while (possible_paths[i])
	{
		absolute_path = (ft_strjoin(possible_paths[i], cmd_info->full_cmd[0]));
		if (access(absolute_path, X_OK) == 0)
		{
			free(cmd_info->full_cmd[0]);
			cmd_info->full_cmd[0] = absolute_path;
			break ;
		}
		i++;
		free(absolute_path);
	}
	if (!possible_paths[i])
	{
		ft_dprintf(2, "pipex: %s: command not found\n",
			cmd_info->relative_path);
		free_str_arr(possible_paths);
		return (1);
	}
	return (0);
}

static int	manage_cmd_str(t_cmd *cmd_info, char *envp[])
{
	size_t	i;
	size_t	j;
	char	**possible_paths;

	if (parse_path_env_var(&i, &j, envp, *cmd_info) != 0)
		return (1);
	possible_paths = ft_split(&envp[i][j], ':');
	cmd_info->full_cmd[0] = ft_strjoin("/", cmd_info->full_cmd[0]);
	if (look_for_valid_path(possible_paths, cmd_info) != 0)
		return (2);
	free_str_arr(possible_paths);
	return (0);
}

static void	run_rel_path_cmd(t_cmd cmd_info, t_files files_info,
	char *envp[])
{
	if (execve(cmd_info.full_cmd[0], cmd_info.full_cmd, envp) == -1)
	{
		managerr(2, cmd_info.relative_path, files_info);
		free(cmd_info.relative_path);
		free_str_arr(cmd_info.full_cmd);
		exit(EXIT_FAILURE);
	}
	return ;
}

void	manage_cmd(char **full_cmd, t_files files_info, char *envp[])
{
	t_cmd	cmd_info;
	int		str_error;

	if (ft_strchr(full_cmd[0], '/') != NULL)
		run_abs_path_cmd(full_cmd, files_info, envp);
	else
	{
		cmd_info.relative_path = full_cmd[0];
		cmd_info.full_cmd = full_cmd;
		str_error = manage_cmd_str(&cmd_info, envp);
		if (str_error == 0)
			run_rel_path_cmd(cmd_info, files_info, envp);
		else if (str_error == 2)
			free(cmd_info.relative_path);
	}
	free_str_arr(full_cmd);
	exit(EXIT_FAILURE);
}
