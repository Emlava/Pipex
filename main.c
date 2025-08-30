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

static void	manage_files(t_files *files_info, t_startup s_resources)
{
	files_info->infile_path = s_resources.av[1];
	files_info->outfile_path = s_resources.av[s_resources.ac - 1];
	open_files(files_info);
	files_info->dev_null_fd = -1;
	return ;
}

static void	run_child(t_processes p_resources, t_files *files_info,
	size_t *i, t_startup s_resources)
{
	close(p_resources.pipe_fds[0]);
	manage_input_src(files_info, i, p_resources);
	manage_output_dst(files_info, *i, s_resources.ac,
		p_resources.pipe_fds[1]);
	manage_cmd(ft_split(s_resources.av[*i], ' '), *files_info,
		s_resources.envp);
	return ;
}

static void	manage_pipe_in_parent(t_processes *p_resources, size_t *i,
	t_files files_info, int ac)
{
	close(p_resources->pipe_fds[1]);
	wait(NULL);
	if (*i == 2 && files_info.infile_fd == -1)
		*i = 3;
	if ((*i == 3 && files_info.infile_fd != -1) || (*i > 3))
		close(p_resources->prev_read_end);
	if ((int)*i != ac - 2)
		p_resources->prev_read_end = p_resources->pipe_fds[0];
	else
		close(p_resources->pipe_fds[0]);
	(*i)++;
	return ;
}

int	main(int ac, char *av[], char *envp[])
{
	t_startup	s_resources;
	t_files		files_info;
	size_t		i;
	t_processes	p_resources;

	if (ac < 5)
	{
		ft_dprintf(2, "pipex: at least 4 arguments needed\n");
		exit(EXIT_FAILURE);
	}
	s_resources.ac = ac;
	s_resources.av = av;
	s_resources.envp = envp;
	manage_files(&files_info, s_resources);
	i = 2;
	while ((int)i < ac - 1)
	{
		open_pipe_and_fork(&p_resources, files_info);
		if (p_resources.pid == 0)
			run_child(p_resources, &files_info, &i, s_resources);
		else
			manage_pipe_in_parent(&p_resources, &i, files_info, s_resources.ac);
	}
	close_all_fds(files_info);
	return (0);
}
