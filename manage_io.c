/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_io.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.s19.be>         #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-25 15:20:15 by elara-va          #+#    #+#             */
/*   Updated: 2025-08-25 15:20:15 by elara-va         ###   ########.be       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void instance_1(t_files *files_info, int pipe_fds[], size_t *i, int *errors)
{
	if (files_info->infile_fd != -1)
	{
		if (dup2(files_info->infile_fd, STDIN_FILENO) == -1)
			(*errors)++;
		else
		{
			close(files_info->infile_fd);
			files_info->infile_fd = -1;
		}
	}
	else
	{
		*i = 3;
		files_info->dev_null_fd = open("/dev/null", O_RDWR);
		if (files_info->dev_null_fd == -1)
		{
			managerr(2, "/dev/null");
			close_pipe(pipe_fds);
			exit(EXIT_FAILURE);
		}
		if (dup2(files_info->dev_null_fd, STDIN_FILENO) == -1)
			(*errors)++;
		else
		{
			close(files_info->dev_null_fd);
			files_info->dev_null_fd = -1;
		}
	}
	return ;
}

static void instance_2(t_files *files_info, int pipe_fds[], int *errors)
{
	if (files_info->dev_null_fd != -1)
	{
		close(files_info->dev_null_fd);
		files_info->dev_null_fd = -1;
	}
	if (dup2(pipe_fds[0], STDIN_FILENO) == -1) // We need this every time from cmd2 (i = 3)
	// if infile exists or from cmd3 (i = 4) if infile doesn't exist
		(*errors)++;
	else
	{
		close(pipe_fds[0]);
		pipe_fds[0] = -1;
	}
	return ;
}

static void	instance_3(int pipe_fds[], int *errors)
{
	if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
		(*errors)++;
	else
	{
		close(pipe_fds[1]);
		pipe_fds[1] = -1;
	}
	return ;
}

static void instance_4(t_files *files_info, int *errors, int *return_value)
{
	if (files_info->outfile_fd != -1)
	{
		if (dup2(files_info->outfile_fd, STDOUT_FILENO) == -1)
			(*errors)++;
		else
		{
			close(files_info->outfile_fd);
			files_info->outfile_fd = -1;
		}
	}
	else
		*return_value = 1;
	return ;
}

// Instance 1:
// Manage stdin for the first command if infile exists, or ignore the command
// if infile doesn't exist by setting the value of i to 3 and setting /dev/null
// as stdin for the next command.
//
// Instance 2:
// Manage stdin for the second command if infile exists or for the third command
// if infile doesn't exist; close /dev/null if it's open. Pass NULL as last argument.
//
// Instance 3:
// stdout is set to pipe_fds[1].
//
// Instance 4:
// Manage stdout for the last command if outfile exists, or return 1
// if outfile doesn't exist. Pass NULL as last argument.
int	manage_io(int instance, t_files *files_info, int pipe_fds[], size_t *i)
{
	int	errors;
	int	return_value;

	errors = 0;
	return_value = 0;
	if (instance == 1)
		instance_1(files_info, pipe_fds, i, &errors);
	else if (instance == 2)
		instance_2(files_info, pipe_fds, &errors);
	else if (instance == 3)
		instance_3(pipe_fds, &errors);
	else if (instance == 4)
		instance_4(files_info, &errors, &return_value);
	if (errors)
	{
		close_pipe(pipe_fds);
		managerr(3, "dup2()", files_info);
	}
	return (return_value);
}