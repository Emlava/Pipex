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

static void	manage_input_src_start(t_files *files_info, size_t *i, int p_write_end, int *errors)
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
		files_info->dev_null_fd = open("/dev/null", O_RDWR); // See later on if we can simply close
		// the write end and not use /dev/null
		if (files_info->dev_null_fd == -1)
		{
			managerr(2, "/dev/null");
			close(p_write_end);
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

static void	manage_input_src_ongoing(t_files *files_info, int p_prev_read_end, int *errors)
{
	if (files_info->dev_null_fd != -1)
	{
		close(files_info->dev_null_fd);
		files_info->dev_null_fd = -1;
	}
	if (dup2(p_prev_read_end, STDIN_FILENO) == -1)
		(*errors)++;
	else
		close(p_prev_read_end);
	return ;
}

void	manage_input_src(t_files *files_info, size_t *i, t_processes p_resources)
{
	int	errors;

	errors = 0;
	if (*i == 2)
		manage_input_src_start(files_info, i, p_resources.pipe_fds[1], &errors);
	else
		manage_input_src_ongoing(files_info, p_resources.prev_read_end, &errors);
	if (errors)
	{
		close(p_resources.prev_read_end);
		close(p_resources.pipe_fds[1]);
		managerr(3, "dup2()", *files_info);
	}
	return ;
}

int	manage_output_dst(t_files *files_info, size_t i, int ac, int p_write_end)
{
	int	errors;
	int	return_value;

	errors = 0;
	return_value = 0;
	if ((int)i != ac - 2)
	{
		if (dup2(p_write_end, STDOUT_FILENO) == -1)
			errors++;
		else
			close(p_write_end);
	}
	else if (files_info->outfile_fd != -1)
	{
		{
			if (dup2(files_info->outfile_fd, STDOUT_FILENO) == -1)
				errors++;
			else
			{
				close(files_info->outfile_fd);
				files_info->outfile_fd = -1;
			}
		}
	}
	else
		return_value = 1;
	if (errors)
	{
		close(p_write_end);
		managerr(3, "dup2()", *files_info);
	}
	return (return_value);
}
