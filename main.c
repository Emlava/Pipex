/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.s19.be>         #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-06 16:09:33 by elara-va          #+#    #+#             */
/*   Updated: 2025-08-06 16:09:33 by elara-va         ###   ########.be       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char *av[], char *envp[])
{
	t_files	files_info;
	size_t	i;
	int		pipe_fds[2];
	int		pid;
	int		p_prev_read_end;

	if (ac < 5)
	{
		ft_dprintf(2, "pipex: at least 4 arguments needed\n");
		exit(EXIT_FAILURE);
	}
	files_info.infile_path = av[1];
	files_info.outfile_path = av[ac - 1];
	open_files(&files_info);
	files_info.dev_null_fd = -1;
	i = 2;
	while ((int)i < ac - 1)
	{
		open_pipe(pipe_fds, files_info);
		pid = fork();
		if (pid == -1)
		{
			close_pipe(pipe_fds);
			managerr(3, "fork()", files_info);
		}
		if (pid == 0) // CHILD
		{
			close(pipe_fds[0]);
			manage_input_src(&files_info, &i, pipe_fds[1], p_prev_read_end);
			if (manage_output_dst(&files_info, i, ac, pipe_fds[1]) == 1)
				break ;
			manage_cmd(ft_split(av[i], ' '), files_info, envp);
		}
		else // PARENT
		{
			close(pipe_fds[1]);
			wait(NULL);
			if (i == 2 && files_info.infile_fd == -1)
				i = 3;
			if ((i == 3 && files_info.infile_fd != -1) || (i > 3))
				close(p_prev_read_end);
			if ((int)i != ac - 2)
				p_prev_read_end = pipe_fds[0];
			else
				close(pipe_fds[0]);
			i++;
		}
	}
	close_all_fds(files_info);
	return (0);
}
