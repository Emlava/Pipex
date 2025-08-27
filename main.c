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
		if (i == 2)
			manage_io(1, &files_info, pipe_fds, &i);
		if (i >= 5 || (i == 3 && files_info.infile_fd != -1) // cmd2 and infile exists 
			|| (i == 4 && files_info.infile_fd == -1)) // cmd3 and infile doesn't exist
			manage_io(2, &files_info, pipe_fds, NULL);
		if ((int)i != ac - 2)
			manage_io(3, &files_info, pipe_fds, NULL);
		else if (manage_io(4, &files_info, pipe_fds, NULL) == 1)
			break ;
		manage_cmd(ft_split(av[i], ' '), files_info, envp, pipe_fds); // Manage closing of pipe ends here
		i++;
		close_pipe(pipe_fds);
	}
	close_all_fds(files_info);
	return (0);
}
