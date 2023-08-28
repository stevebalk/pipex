/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 13:58:52 by sbalk             #+#    #+#             */
/*   Updated: 2023/08/28 18:06:35 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>


void	free_pipex_struct(t_pipex *pipex)
{
	ft_free_2darray((void***) pipex->cmd_args);
	ft_free_array((void**) pipex->env_paths);
}

void	error_exit(t_pipex *pipex, char *msg, int error_number)
{
	if (pipex)
		free_pipex_struct(pipex);
	if (msg)
		ft_putendl_fd(msg, 2);
		// write(2, msg, ft_strlen(msg));
	if (error_number)
		exit(error_number);
	exit(EXIT_FAILURE);
}

char	*get_command(t_pipex *pipex, int command)
{
	char	*ret;
	size_t	i;

	i = 0;
	while (pipex->env_paths[i])
	{
		ret = ft_strjoin(pipex->env_paths[i], pipex->cmd_args[command][0]);
		if (access(ret, F_OK | X_OK) == 0)
		{
			ft_free_array((void **) pipex->env_paths);
			return (ret);
		}
		free(ret);
		i++;
	}
	return (pipex->cmd_args[command][0]);
}


void	parent(t_pipex *pipex, int fd[2], int pid)
{
	char	*command;

	waitpid(pid, NULL, 0);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	dup2(pipex->out_fd, STDOUT_FILENO);
	command = get_command(pipex, 1);
	if (execve(command, pipex->cmd_args[1], pipex->envp) == -1)
	{
		perror(command);
		ft_free_array((void*) command);
		error_exit(pipex, NULL, errno);
	}
}

void	child(t_pipex *pipex, int fd[2])
{
	char	*command;

	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	dup2(pipex->in_fd, STDIN_FILENO);
	command = get_command(pipex, 0);
	if (execve(command, pipex->cmd_args[0], pipex->envp) == -1)
	{
		perror(command);
		ft_free_array((void*) command);
		error_exit(pipex, NULL, errno);
	}
}

void	execute(t_pipex *pipex)
{
	int		pid;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		perror("Pipe: ");
		error_exit(pipex, NULL, errno);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Fork: ");
		error_exit(pipex, NULL, errno);
	}
	if (pid == 0)
		child(pipex, fd);
	parent(pipex, fd, pid);
}

void	get_cmds(t_pipex *pipex, int argc, char **argv)
{
	int		i;
	int		j;

	i = 1;
	j = 0;
	pipex->cmd_args = calloc((argc - 2) + 1, sizeof(char *));
	if (pipex->cmd_args == NULL)
			error_exit(pipex, ERR_NOMEM, errno);
	while (i < argc - 1)
	{
		pipex->cmd_args[j] = ft_split(argv[i], ' ');
		if (pipex->cmd_args[j] == NULL)
			error_exit(pipex, ERR_NOMEM, errno);
		j++;
		i++;
	}
}

static void	add_slash_to_path(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (pipex->env_paths[i] != NULL)
		{
			char *temp = pipex->env_paths[i];
			pipex->env_paths[i] = ft_strjoin(pipex->env_paths[i], "/");
			free(temp);
			if (pipex->env_paths[i] == NULL)
				error_exit(pipex, ERR_NOMEM, errno);
			i++;
		}
}


static void	get_env_paths(t_pipex *pipex, char **envp)
{
	int		i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			pipex->env_paths = ft_split(envp[i] + 5, ':');
			if (pipex->env_paths == NULL)
				error_exit(pipex, ERR_NOMEM, errno);
			add_slash_to_path(pipex);
			break;
		}
		i++;
	}
}

void	open_files(t_pipex *pipex, int argc, char **argv)
{
	pipex->in_fd = open(argv[0], O_RDONLY, 0644);
	if (pipex->in_fd == -1)
	{
		perror(pipex->cmd_args[0][0]);
		exit(errno);
	}
	pipex->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->out_fd == -1)
	{
		perror(pipex->cmd_args[1][0]);
		exit(errno);
	}
}


int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pipex;

	pipex.envp = envp;
	argc--;
	argv++;
	if (argc < 4)
	{
		ft_putstr_fd("Not enough arguments (min 4)\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (ft_streq("here_doc", argv[1]) && argc == 6)
		return (1);
	get_env_paths(&pipex, envp);
	get_cmds(&pipex, argc, argv);
	open_files(&pipex, argc, argv);
	execute(&pipex);
	free_pipex_struct(&pipex);
}
