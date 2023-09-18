/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 15:42:58 by sbalk             #+#    #+#             */
/*   Updated: 2023/09/18 14:15:24 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

# define CMD_0 = 0
# define CMD_1 = 1

typedef struct s_pipex
{
	char	*prog_name;
	int		in_fd;
	int		out_fd;
	char	*infile;
	char	*outfile;
	char	**env_paths;
	char	***cmd_args;
	char	**cmd_paths;
	size_t	command_count;
	char	**envp;
}			t_pipex;

char	**parse_quote_cmd(t_pipex *pipex, char *str);
void	get_env_paths(t_pipex *pipex);
void	get_cmds(t_pipex *pipex, int argc, char **argv);
void	execute(t_pipex *pipex);
void	handle_error(t_pipex *pipex, char *msg, int is_stderr, int shall_exit);
void	free_pipex_struct(t_pipex *pipex);
void	get_cmd_paths(t_pipex *p);

#endif