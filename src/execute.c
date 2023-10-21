/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 13:48:32 by sbalk             #+#    #+#             */
/*   Updated: 2023/10/21 13:14:35 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	parent(t_pipex *pipex, int fd[2])
{
	if (dup2(fd[0], STDIN_FILENO) == -1)
		handle_error(pipex, "dup2 (in): parent:", 1, 1);
	pipex->out_fd = open(pipex->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->out_fd == -1)
		handle_error(pipex, "could not create file:", 1, 1);
	if (dup2(pipex->out_fd, STDOUT_FILENO) == -1)
		handle_error(pipex, "dup2 (out): parent:", 1, 1);
	close(fd[1]);
	close(pipex->out_fd);
	if (pipex->cmd_paths[1] != NULL)
		if (execve(pipex->cmd_paths[1], pipex->cmd_args[1], pipex->envp) == -1)
			handle_error(pipex, "parent: execve:", 1, 1);
	free_pipex_struct(pipex);
	exit(127);
}

static void	child(t_pipex *pipex, int fd[2])
{
	if (pipex->cmd_paths[0] == NULL)
		return ;
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		handle_error(pipex, "dup2 (out): child:", 1, 1);
	pipex->in_fd = open(pipex->infile, O_RDONLY, 0644);
	if (pipex->in_fd == -1)
		handle_error(pipex, "input:", 1, 1);
	if (dup2(pipex->in_fd, STDIN_FILENO) == -1)
		handle_error(pipex, "dup2 (in): child:", 1, 1);
	close(fd[0]);
	close(pipex->in_fd);
	if (execve(pipex->cmd_paths[0], pipex->cmd_args[0], pipex->envp) == -1)
		handle_error(pipex, "child: execve", 1, 0);
}

void	execute(t_pipex *pipex)
{
	int		pid;
	int		fd[2];
	int		stat_loc;

	stat_loc = 0;
	if (pipe(fd) == -1)
		handle_error(pipex, "execute: pipe:", 1, 1);
	pid = fork();
	if (pid == -1)
		handle_error(pipex, "execute: fork:", 1, 1);
	if (pid == 0)
		child(pipex, fd);
	if (pid > 0)
	{
		if (waitpid(pid, &stat_loc, WNOHANG) == 0)
		{
			if (WIFEXITED(stat_loc) && WEXITSTATUS(stat_loc) != 0)
				exit(WEXITSTATUS(stat_loc));
		}
		parent(pipex, fd);
	}
}
