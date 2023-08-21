/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 13:58:52 by sbalk             #+#    #+#             */
/*   Updated: 2023/08/21 18:14:35 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>


void	free_pipex_struct(t_pipex *pipex)
{
	ft_free_2darray((void***) pipex->cmd_args);
	ft_free_array((void**) pipex->env_paths);
}

void	error_exit(t_pipex *pipex, char *msg)
{
	if (pipex)
		free_pipex_struct(pipex);
	if (msg)
		write(2, msg, ft_strlen(msg));
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
		perror(ret);
		free(ret);
		i++;
	}
	perror("Get command: ");
	error_exit(pipex, NULL);
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
		error_exit(pipex, NULL);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("Fork: ");
		error_exit(pipex, NULL);
	}
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		command = get_command(pipex, pipex->cmd_args[0][0]);
		execv(command, pipex->cmd_args[0]);
	}
	else
	{
		waitpid(pid1, 0, 0);
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		command = get_command(pipex, pipex->cmd_args[1][0]);
		execv(command, pipex->cmd_args[1]);
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
			error_exit(pipex, ERR_NOMEM);
	while (i < argc - 1)
	{
		pipex->cmd_args[j] = ft_split(argv[i], ' ');
		if (pipex->cmd_args[j] == NULL)
			error_exit(pipex, ERR_NOMEM);
		printf("%s\n", pipex->cmd_args[j][0]);
		fflush(stdout);
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
				error_exit(pipex, ERR_NOMEM);
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
				error_exit(pipex, ERR_NOMEM);
			add_slash_to_path(pipex);
			break;
		}
		i++;
	}
}

void	open_files(t_pipex *pipex, int argc, char **argv)
{
	pipex->in_fd = open(argv[0], O_RDONLY);
	if (pipex->in_fd == -1)
	{
		perror("Infile: ");
		exit(EXIT_FAILURE);
	}
	// ft_printf("IN1: %i\n", pipex->in_fd);
	// dup2(pipex->in_fd, STDIN_FILENO);
	// close(pipex->in_fd);
	// ft_printf("IN2: %i\n", pipex->in_fd);
	pipex->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
	if (pipex->out_fd == -1)
	{
		perror("Outfile: ");
		exit(EXIT_FAILURE);
	}
	// dup2(pipex->out_fd, STDOUT_FILENO);
	// close(pipex->out_fd);
}

// void	init_struct(t_pipex *px, int argc, char **argv, char **envp)
// {
// 	px->infile = argv[1];
// 	px->outfile = argv[argc - 1];
// 	px->cmd_count = argc - 3;
// 	px->here_doc = ft_streq("here_doc", argv[1]);
// 	px->is_invalid_infile = access(argv[1], R_OK);
// 	px->
// }

int	main(int argc, char *argv[], char *envp[])
{
	// t_pipex	px;

	// if (argc <= 4)
	// {
	// 	ft_putnchar_fd("Not enough arguments (min 4)\n", STDERR_FILENO, 29);
	// 	return (EXIT_FAILURE);
	// }
	// else if (ft_streq("here_doc", argv[1]) && argc == 6)
	// 	return ;
	// else
	// init_struct(&px, argc, argv, envp);

	t_pipex	pipex;

	argc--;
	argv++;
	open_files(&pipex, argc, argv);
	get_env_paths(&pipex, envp);
	get_cmds(&pipex, argc, argv);
	child1(&pipex);
	// ft_printf("%s", pipex.cmd_args[0][0]);
	free_pipex_struct(&pipex);
}
