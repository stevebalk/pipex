/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_one.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 13:44:01 by sbalk             #+#    #+#             */
/*   Updated: 2023/09/13 15:28:15 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	has_quote(char *str)
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

void	get_cmds(t_pipex *pipex, int argc, char **argv)
{
	size_t		i;
	size_t		j;

	i = 1;
	j = 0;
	pipex->command_count = argc - 2;
	pipex->cmd_args = ft_calloc((argc - 2) + 1, sizeof(char *));
	if (pipex->cmd_args == NULL)
		handle_error(pipex, "calloc: get_cmds:", 1, 1);
	while (i <= pipex->command_count)
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
	ft_putstr_fd(ret, 2);
	free(ret);
	return (NULL);
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

void	get_env_paths(t_pipex *pipex)
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
