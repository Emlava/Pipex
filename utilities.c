/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.s19.be>         #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-20 12:57:43 by elara-va          #+#    #+#             */
/*   Updated: 2025-08-20 12:57:43 by elara-va         ###   ########.be       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	open_files(t_files *files_info)
{
	files_info->outfile_fd = open(files_info->outfile_path,
			O_WRONLY | O_TRUNC | O_CREAT, 00666);
	if (files_info->outfile_fd == -1)
		managerr(1, files_info->outfile_path);
	files_info->infile_fd = open(files_info->infile_path, O_RDONLY);
	if (files_info->infile_fd == -1)
			managerr(1, files_info->infile_path);
	return ;
}

// instance 1 to print strerror() without exiting:
// pass only the name of the relevant file or function after instance
//
// instance 2 to print strerror() and close fds without exiting:
// pass the name of the relevant file or function and files_info after instance
//
// instance 3 to print strerror(), close fds and exit:
// pass the name of the relevant file or function and files_info after instance
void	managerr(int instance, ...)
{
	va_list	args;

	va_start(args, instance);
	ft_dprintf(2, "pipex: %s: %s\n", va_arg(args, char *), strerror(errno));
	if (instance == 1)
		va_end(args);
	if (instance == 2 || instance == 3)
	{
		close_all_fds(va_arg(args, t_files));
		va_end(args);
		if (instance == 3)
			exit(EXIT_FAILURE);
	}
	return ;
}

void	open_pipe(int pipe_fds[], t_files files_info)
{
	if (pipe(pipe_fds) == -1)
	{
		managerr(3, "pipe()", files_info);
		close_all_fds(files_info);
	}
	return ;
}

void	free_str_arr(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return ;
}


