/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_all_fds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.s19.be>         #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-22 12:40:05 by elara-va          #+#    #+#             */
/*   Updated: 2025-08-22 12:40:05 by elara-va         ###   ########.be       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_pipe(int pipe_fds[])
{
	if (pipe_fds[0] != -1)
		close(pipe_fds[0]);
	if (pipe_fds[1] != -1)
		close(pipe_fds[1]);
	return ;
}

static void	close_files(t_files files_info)
{
	if (files_info.dev_null_fd != -1)
		close(files_info.dev_null_fd);
	if (files_info.infile_fd != -1)
		close(files_info.infile_fd);
	if (files_info.outfile_fd != -1)
		close(files_info.outfile_fd);
	return ;
}

void	close_all_fds(t_files files_info)
{
	close_files(files_info);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	return ;
}
