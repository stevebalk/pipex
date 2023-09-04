/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 13:58:52 by sbalk             #+#    #+#             */
/*   Updated: 2023/09/04 17:28:58 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	free_pipex_struct(t_pipex *pipex)
{
	ft_free_2darray((void ***) pipex->cmd_args);
	ft_free_array((void **) pipex->env_paths);
}

void	handle_error(t_pipex *pipex, char *msg, int is_stderr, int shall_exit)
{
	if (msg)
	{
		if (is_stderr)
		{
			ft_putstr_fd(pipex->prog_name, 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(msg, 2);
			ft_putstr_fd(" ", 2);
			perror("");
		}
		else
		{
			ft_putstr_fd(pipex->prog_name, 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(msg, 2);
			ft_putstr_fd("\n", 2);
		}
	}
	if (shall_exit)
	{
		free_pipex_struct(pipex);
		exit(errno);
	}
}

// int	is_awk(char *str)
// {
// 	while (ft_is_space(str))
// 		str++;
// 	if (ft_strncmp("awk ", str, 4) == 0)
// 		return (1);
// 	return (0);
// }

// char	**parse_awk(t_pipex *pipex, char *str)
// {
// 	char	**ret;

// 	ret = ft_calloc(3, sizeof(char *));
// 	if (ret == NULL)
// 		handle_error(pipex, "calloc: parse awk: ", 1, 1);
// 	ret[0] = ft_strdup("awk");
// 	str = ft_strnstr(str, "awk", ft_strlen(str));
// 	if (str != NULL)
// 		str += 3;
// 	while (ft_is_space(str))
// 		str++;
// 	if (*str == '\'')
// 		ret[1] = ft_strtrim(str, "\'");
// 	else
// 		ret[1] = ft_strtrim(str, "\"");
// 	if (ret[1] == NULL)
// 		handle_error(pipex, "ft_strtrim: parse awk: ", 1, 1);
// 	return (ret);
// }

// int	has_quote(char *str)
// {
// 	char	quote;
// 	size_t	len;

// 	quote = ' ';
// 	while (ft_is_space(str) && *str != '\0')
// 		str++;
// 	while (!ft_is_space(str) && *str != '\0')
// 		str++;
// 	while (ft_is_space(str) && *str != '\0')
// 		str++;
// 	if (*str == '\'' || *str == '\"')
// 		quote = *str;
// 	else
// 		return (0);
// 	len = ft_strlen(str);
// 	while (ft_is_space(&str[len - 1]) && len > 1)
// 		len--;
// 	if (str[len - 1] == quote && len > 1)
// 		return (1);
// 	return (0);
// }

// char	*get_quote_command(t_pipex *pipex, char **str)
// {
// 	size_t	len;
// 	char	*ret;

// 	len = 0;
// 	while (ft_is_space(*str))
// 		(*str)++;
// 	while (!ft_is_space(*str + len))
// 		len++;
// 	ret = ft_calloc(len + 1, sizeof(char));
// 	if (ret == NULL)
// 		handle_error(pipex, "calloc: get_quote_cmd:", 1, 1);
// 	ft_strlcpy(ret, *str, len + 1);
// 	*str += len;
// 	return (ret);
// }

// char	**parse_quote_cmd(t_pipex *pipex, char *str)
// {
// 	char	**ret;
// 	size_t	last_quote_pos;

// 	ret = ft_calloc(3, sizeof(char *));
// 	if (ret == NULL)
// 		handle_error(pipex, "calloc: parse awk: ", 1, 1);
// 	ret[0] = get_quote_command(pipex, &str);
// 	last_quote_pos = ft_strlen(str);
// 	while (str[last_quote_pos - 1] != *str)
// 		last_quote_pos--;
// 	ret[1] = ft_substr(str, 0, last_quote_pos - 2);
// 	// while (ft_is_space(str) && *str != '\0')
// 	// 	str++;
// 	// if (*str == '\'')
// 	// 	ret[1] = ft_strtrim(str, "\'");
// 	// else
// 	// 	ret[1] = ft_strtrim(str, "\"");
// 	if (ret[1] == NULL)
// 		handle_error(pipex, "ft_strtrim: parse_quote_cmd: ", 1, 1);
// 	return (ret);
// }

int	has_quote(char *str)
{
	while (ft_is_space(str) && *str != '\0')
		str++;
	while (!ft_is_space(str) && *str != '\0')
		str++;
	while (ft_is_space(str) && *str != '\0')
		str++;
	if (*str == '\'' || *str == '\"')
		return (1);
	return (0);
}

char	*get_quote_command(t_pipex *pipex, char **str)
{
	size_t	len;
	char	*ret;

	len = 0;
	while (ft_is_space(*str))
		(*str)++;
	while (!ft_is_space(*str + len))
		len++;
	ret = ft_calloc(len + 1, sizeof(char));
	if (ret == NULL)
		handle_error(pipex, "calloc: get_quote_cmd:", 1, 1);
	ft_strlcpy(ret, *str, len + 1);
	*str += len;
	return (ret);
}

char	**parse_quote_cmd(t_pipex *pipex, char *str)
{
	char	**ret;

	ret = ft_calloc(3, sizeof(char *));
	if (ret == NULL)
		handle_error(pipex, "calloc: parse awk: ", 1, 1);
	ret[0] = get_quote_command(pipex, &str);
	while (ft_is_space(str) && *str != '\0')
		str++;
	if (*str == '\'')
		ret[1] = ft_strtrim(str, "\'");
	else
		ret[1] = ft_strtrim(str, "\"");
	if (ret[1] == NULL)
		handle_error(pipex, "ft_strtrim: parse_quote_cmd: ", 1, 1);
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
			return (ret);
		free(ret);
		i++;
	}
	ret = ft_strjoin(pipex->cmd_args[command][0], ": command not found");
	handle_error(pipex, ret, 0, 0);
	free(ret);
	return (NULL);
}

void	parent(t_pipex *pipex, int fd[2], int pid, char *command)
{
	int		stat_loc;

	waitpid(pid, &stat_loc, WNOHANG);
	if (WIFEXITED(stat_loc) && WEXITSTATUS(stat_loc) != 0)
		exit(WEXITSTATUS(stat_loc));
	if (dup2(fd[0], STDIN_FILENO) == -1)
		handle_error(pipex, "dup2 (in): parent:", 1, 1);
	pipex->out_fd = open(pipex->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->out_fd == -1)
		handle_error(pipex, "could not create file:", 1, 1);
	if (dup2(pipex->out_fd, STDOUT_FILENO) == -1)
		handle_error(pipex, "dup2 (out): parent:", 1, 1);
	close(fd[1]);
	close(pipex->out_fd);
	if (command != NULL)
	{
		if (execve(command, pipex->cmd_args[1], pipex->envp) == -1)
		{
			ft_free_array((void *) command);
			handle_error(pipex, "parent: execve:", 1, 1);
		}
	}
	free_pipex_struct(pipex);
	exit(127);
}

void	child(t_pipex *pipex, int fd[2], char *command)
{
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		handle_error(pipex, "dup2 (out): child:", 1, 1);
	// close(fd[1]);
	pipex->in_fd = open(pipex->infile, O_RDONLY, 0644);
	if (pipex->in_fd == -1)
		handle_error(pipex, "input:", 1, 1);
	if (dup2(pipex->in_fd, STDIN_FILENO) == -1)
		handle_error(pipex, "dup2 (in): child:", 1, 1);
	close(fd[0]);
	close(pipex->in_fd);
	if (command != NULL)
	{
		if (execve(command, pipex->cmd_args[0], pipex->envp) == -1)
		{
			handle_error(pipex, "child: execve", 1, 0);
			ft_free_array((void *) command);
		}
	}
}

void	execute(t_pipex *pipex)
{
	int		pid;
	int		fd[2];

	if (pipe(fd) == -1)
		handle_error(pipex, "execute: pipe:", 1, 1);
	pid = fork();
	if (pid == -1)
		handle_error(pipex, "execute: fork:", 1, 1);
	if (pid == 0)
		child(pipex, fd, get_command(pipex, 0));
	else
		parent(pipex, fd, pid, get_command(pipex, 1));
}

void	get_cmds(t_pipex *pipex, int argc, char **argv)
{
	int		i;
	int		j;

	i = 1;
	j = 0;
	pipex->cmd_args = ft_calloc((argc - 2) + 1, sizeof(char *));
	if (pipex->cmd_args == NULL)
		handle_error(pipex, "calloc: get_cmds:", 1, 1);
	while (i < argc - 1)
	{
		if (has_quote(argv[i]))
			pipex->cmd_args[j] = parse_quote_cmd(pipex, argv[i]);
		else
			pipex->cmd_args[j] = ft_split(argv[i], ' ');
		if (pipex->cmd_args[j] == NULL)
			handle_error(pipex, "ft_split: get_cmds:", 1, 1);
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
			handle_error(pipex, "ft_strjoin: add_slash:", 1, 1);
		i++;
	}
}

static void	get_env_paths(t_pipex *pipex)
{
	int		i;
	char	*defp;

	i = 0;
	defp = "PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin";
	if (pipex->envp[0] == NULL)
		pipex->env_paths = ft_split(defp, ':');
	while (pipex->envp[0] != NULL)
	{
		if (ft_strncmp("PATH=", pipex->envp[i], 5) == 0)
		{
			pipex->env_paths = ft_split(pipex->envp[i] + 5, ':');
			if (pipex->env_paths == NULL)
				handle_error(pipex, "ft_split: get_env_paths:", 1, 1);
			break ;
		}
		i++;
	}
	add_slash_to_path(pipex);
}

void	intitalize_struct(int *argc, char **argv[], t_pipex *pipex, char *envp[])
{
	pipex->envp = envp;
	pipex->prog_name = (*argv)[0];
	pipex->prog_name += 2;
	(*argc)--;
	(*argv)++;
	pipex->infile = (*argv)[0];
	pipex->outfile = (*argv)[*argc - 1];
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pipex;

	if (argc != 5)
	{
		ft_putendl_fd("Wrong number of arguments:", STDERR_FILENO);
		ft_putendl_fd("<infile> <cmd1> <cmd2> <outfile>", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	intitalize_struct(&argc, &argv, &pipex, envp);
	get_env_paths(&pipex);
	get_cmds(&pipex, argc, argv);
	execute(&pipex);
	free_pipex_struct(&pipex);
}
