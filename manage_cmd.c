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

static int	parse_cmd_str(t_cmd *cmd_info, char *envp[]) // sets full_cmd[0] to the absolute path
{
	size_t	i;
	size_t	j;
	char	**possible_paths;
	char	*absolute_path;

	i = 0;
	j = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
	{
		ft_dprintf(2, "pipex: %s: No such file or directory\n", cmd_info->relative_path);
		return (1);
	}
	else
	{
		while (envp[i][j] && envp[i][j] != '/')
			j++;
		if (!envp[i][j])
		{
			ft_dprintf(2, "pipex: %s: No such file or directory\n", cmd_info->relative_path);
			return (2);
		}
	}
	possible_paths = ft_split(&envp[i][j], ':');
	cmd_info->full_cmd[0] = ft_strjoin("/", cmd_info->full_cmd[0]); // This is freed two lines above the break below
	i = 0;
	while (possible_paths[i])
	{
		absolute_path = (ft_strjoin(possible_paths[i], cmd_info->full_cmd[0]));
		if (access(absolute_path, X_OK) == 0)
		{
			free(cmd_info->full_cmd[0]);
			cmd_info->full_cmd[0] = absolute_path; // Now absolute_path gets freed when freeing full_cmd
			break ;
		}
		i++;
		free(absolute_path);
	}
	if (!possible_paths[i])
	{
		ft_dprintf(2, "pipex: %s: command not found\n", cmd_info->relative_path);
		free_str_arr(possible_paths);
		return (3);
	}
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
	int		parsing_error;

	if (ft_strchr(full_cmd[0], '/') != NULL)
		run_abs_path_cmd(full_cmd, files_info, envp);
	else
	{
		cmd_info.relative_path = full_cmd[0];
		cmd_info.full_cmd = full_cmd;
		parsing_error = parse_cmd_str(&cmd_info, envp);
		if (!parsing_error)
			run_rel_path_cmd(cmd_info, files_info, envp);
		else if (parsing_error == 3) // Meaning that full_cmd[0] DID change
			free(cmd_info.relative_path); // This frees original first string of full_cmd;
	}
	free_str_arr(full_cmd); // if parse_cmd_str() succeeds, this also frees absolute_path
	exit(EXIT_FAILURE); // Because if execve() ran we wouldn't have reached here
}
