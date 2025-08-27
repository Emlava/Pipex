/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.s19.be>         #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-06 16:09:39 by elara-va          #+#    #+#             */
/*   Updated: 2025-08-06 16:09:39 by elara-va         ###   ########.be       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <errno.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef struct s_files
{
	int		infile_fd;
	int		outfile_fd;
	int		dev_null_fd;
	char	*infile_path;
	char	*outfile_path;
}	t_files;

typedef struct s_cmd
{
	char	*relative_path;
	char	**full_cmd;
}	t_cmd;

// *** utilities.c *** //
void	open_files(t_files *files_info);
void	managerr(int instance, ...);
void	open_pipe(int pipe_fds[], t_files files_info);
void	free_str_arr(char **arr);

// *** manage_io.c *** //
int		manage_io(int instance, t_files *files_info, int pipe_fds[], size_t *i);

// *** manage_cmd.c *** //
void	manage_cmd(char **full_cmd, t_files files_info, char *envp[], int pipe_fds[]);

// *** closing.c *** //
void	close_pipe(int pipe_fds[]);
void	close_all_fds(t_files files_info);

#endif
