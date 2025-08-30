/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_output_dst.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.s19.be>         #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-25 15:20:15 by elara-va          #+#    #+#             */
/*   Updated: 2025-08-25 15:20:15 by elara-va         ###   ########.be       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	redirect_to_pipe(int p_write_end, int *errors)
{
	if (dup2(p_write_end, STDOUT_FILENO) == -1)
		(*errors)++;
	else
		close(p_write_end);
	return ;
}

static void	redirect_to_outfile(t_files *files_info, int *errors)
{
	if (dup2(files_info->outfile_fd, STDOUT_FILENO) == -1)
		(*errors)++;
	else
	{
		close(files_info->outfile_fd);
		files_info->outfile_fd = -1;
	}
	return ;
}

void	manage_output_dst(t_files *files_info, size_t i,
	int ac, int p_write_end)
{
	int	errors;

	errors = 0;
	if ((int)i != ac - 2)
		redirect_to_pipe(p_write_end, &errors);
	else if (files_info->outfile_fd != -1)
		redirect_to_outfile(files_info, &errors);
	else
		exit(EXIT_FAILURE);
	if (errors)
	{
		close(p_write_end);
		managerr(3, "dup2()", *files_info);
	}
	return ;
}
