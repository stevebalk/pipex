/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 13:58:52 by sbalk             #+#    #+#             */
/*   Updated: 2023/08/31 14:02:26 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	free_pipex_struct(t_pipex *pipex)
{
	ft_free_2darray((void ***) pipex->cmd_args);
	ft_free_array((void **) pipex->env_paths);
}

void	error_exit(t_pipex *pipex, char *msg, char *perr, int error_number)
{
	if (pipex)
		free_pipex_struct(pipex);
	if (msg)
		ft_putendl_fd(msg, 2);
	if (perr)
		perror(perr);
	if (error_number)
		exit(error_number);
	exit(EXIT_FAILURE);
}

int	is_awk(char *str)
{
	while (ft_is_space(str))
		str++;
	if (ft_strncmp("awk ", str, 4) == 0)
		return (1);
	return (0);
}

char	**parse_awk(t_pipex *pipex, char *str)
{
	char	**ret;

	ret = calloc(3, sizeof(char *));
	if (ret == NULL)
		error_exit(pipex, ERR_NOMEM, NULL, errno);
	ret[0] = ft_strdup("awk");
	str = ft_strnstr(str, "awk", ft_strlen(str));
	if (str != NULL)
		str += 3;
	while (ft_is_space(str))
		str++;
	if (*str == '\'')
		ret[1] = ft_strtrim(str, "\'");
	else
		ret[1] = ft_strtrim(str, "\"");
	if (ret[1] == NULL)
		error_exit(pipex, ERR_NOMEM, NULL, errno);
	return (ret);
}

char	*get_command(t_pipex *pipex, int command)
{
	char	*ret;
	size_t	i;

	i = 0;
	ret = pipex->cmd_args[command][0];
	if (access(ret, F_OK | X_OK) == 0)
	{
		ft_free_array((void **) pipex->env_paths);
		return (ret);
	}
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
	ft_putendl_fd("Error: command not found", 2);
	error_exit(pipex, NULL, NULL, errno);
	return (NULL);
}

void	parent(t_pipex *pipex, int fd[2], int pid)
{
	char	*command;
	int		stat_loc;

	waitpid(pid, &stat_loc, WNOHANG);
	if (WIFEXITED(stat_loc) && WEXITSTATUS(stat_loc) != 0)
		exit(WEXITSTATUS(stat_loc));
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_exit(pipex, NULL, "dup2 error", errno);
	// close(fd[0]);
	if (dup2(pipex->out_fd, STDOUT_FILENO) == -1)
		error_exit(pipex, NULL, "dup 2 error", errno);
	// dup2(fd[0], STDIN_FILENO);
	// dup2(pipex->out_fd, STDOUT_FILENO);
	close(fd[1]);
	close(pipex->out_fd);
	command = get_command(pipex, 1);
	if (execve(command, pipex->cmd_args[1], pipex->envp) == -1)
	{
		perror("command not found");
		ft_free_array((void *) command);
		error_exit(pipex, NULL, NULL, errno);
	}
}

void	child(t_pipex *pipex, int fd[2])
{
	char	*command;

	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_exit(pipex, NULL, "dup 2 error", errno);
	// close(fd[1]);
	if (dup2(pipex->in_fd, STDIN_FILENO) == -1)
		error_exit(pipex, NULL, "dup 2 error", errno);
	// dup2(fd[1], STDOUT_FILENO);
	// dup2(pipex->in_fd, STDIN_FILENO);
	close(fd[0]);
	close(pipex->in_fd);
	command = get_command(pipex, 0);
	if (execve(command, pipex->cmd_args[0], pipex->envp) == -1)
	{
		perror("Error: command not found");
		ft_free_array((void *) command);
		error_exit(pipex, NULL, NULL, errno);
	}
}

void	execute(t_pipex *pipex)
{
	int		pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error_exit(pipex, NULL, "Pipe: ", errno);
	pid = fork();
	if (pid == -1)
		error_exit(pipex, NULL, "Fork: ", errno);
	if (pid == 0)
		child(pipex, fd);
	else
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
		error_exit(pipex, ERR_NOMEM, NULL, errno);
	while (i < argc - 1)
	{
		if (is_awk(argv[i]))
			pipex->cmd_args[j] = parse_awk(pipex, argv[i]);
		else
			pipex->cmd_args[j] = ft_split(argv[i], ' ');
		if (pipex->cmd_args[j] == NULL)
			error_exit(pipex, ERR_NOMEM, NULL, errno);
		j++;
		i++;
	}
}

static void	add_slash_to_path(t_pipex *pipex)
{
	int		i;
	char	*temp;

	i = 0;
	while (pipex->env_paths[i] != NULL)
	{
		temp = pipex->env_paths[i];
		pipex->env_paths[i] = ft_strjoin(pipex->env_paths[i], "/");
		free(temp);
		if (pipex->env_paths[i] == NULL)
			error_exit(pipex, ERR_NOMEM, NULL, errno);
		i++;
	}
}

static void	get_env_paths(t_pipex *pipex, char **envp)
{
	int		i;

	i = 0;
	if (envp == NULL)
		envp[0] = "PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin";
	while (envp[i] != NULL)
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			pipex->env_paths = ft_split(envp[i] + 5, ':');
			if (pipex->env_paths == NULL)
				error_exit(pipex, ERR_NOMEM, NULL, errno);
			add_slash_to_path(pipex);
			break ;
		}
		i++;
	}
}

void	open_files(t_pipex *pipex, int argc, char **argv)
{
	pipex->in_fd = open(argv[0], O_RDONLY, 0644);
	if (pipex->in_fd == -1)
		perror("input");
		// error_exit(pipex, NULL, pipex->cmd_args[0][0], errno);
	pipex->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->out_fd == -1)
		perror("output");
		// error_exit(pipex, NULL, pipex->cmd_args[1][0], errno);
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
