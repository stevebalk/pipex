/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 13:58:52 by sbalk             #+#    #+#             */
/*   Updated: 2023/08/28 14:59:38 by sbalk            ###   ########.fr       */
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
		write(2, msg, ft_strlen(msg));
	if (error_number)
		exit(error_number);
	exit(EXIT_FAILURE);
}

char	*get_command(t_pipex *pipex, char *command)
{
	char	*ret;
	size_t	i;

	i = 0;
	while (pipex->env_paths[i])
	{
		ret = ft_strjoin(pipex->env_paths[i], command);
		if (access(ret, X_OK) == 0)
			return (ret);
		free(ret);
		i++;
	}
	perror("command not found: ");
	error_exit(pipex, NULL, errno);
	return (NULL);
}

void	child1(t_pipex *pipex)
{
	int		fd[2];
	int		pid1;
	char	*command;

	if (pipe(fd) == -1)
	{
		perror("Pipe: ");
		error_exit(pipex, NULL, errno);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("Fork: ");
		error_exit(pipex, NULL, errno);
	}
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		dup2(pipex->in_fd, STDIN_FILENO);
		command = get_command(pipex, pipex->cmd_args[0][0]);
		execve(command, pipex->cmd_args[0], pipex->envp);
	}
	else
	{
		waitpid(pid1, NULL, 0);
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		dup2(pipex->out_fd, STDOUT_FILENO);
		command = get_command(pipex, pipex->cmd_args[1][0]);
		execve(command, pipex->cmd_args[1], pipex->envp);
	}
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
		perror("Infile");
		exit(errno);
	}
	pipex->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->out_fd == -1)
	{
		perror("Outfile");
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

	// ft_printf("Argc: %d\n", argc);
	open_files(&pipex, argc, argv);
	get_env_paths(&pipex, envp);
	get_cmds(&pipex, argc, argv);
	child1(&pipex);
	free_pipex_struct(&pipex);
}
