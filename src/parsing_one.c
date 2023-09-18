/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_one.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 13:44:01 by sbalk             #+#    #+#             */
/*   Updated: 2023/09/18 17:27:55 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	add_slash_to_paths(t_pipex *pipex, char **paths)
{
	int		i;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(temp);
		if (paths[i++] == NULL)
			handle_error(pipex, "ft_strjoin: add_slash:", 1, 1);
	}
}

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

void	search_valid_cmd_path(t_pipex *p, int i)
{
	int	j;

	j = 0;
	while (p->env_paths[j] != NULL)
	{
		p->cmd_paths[i] = ft_strjoin(p->env_paths[j], p->cmd_args[i][0]);
		if (p->cmd_paths[i] == NULL)
			handle_error(p, "Search_valid_cmd_path: ft_strjoin:", 1, 1);
		if (access(p->cmd_paths[i], F_OK) == 0)
			return ;
		free(p->cmd_paths[i]);
		p->cmd_paths[i] = NULL;
		j++;
	}
}

void	get_cmd_paths(t_pipex *p)
{
	int	i;
	int	j;
	char *error_msg;

	i = 0;
	p->cmd_paths = ft_calloc(sizeof(char *), p->command_count + 1);
	if (p->cmd_paths == NULL)
		handle_error(p, "get_cmd_paths: calloc:", 1, 1);
	while (p->cmd_args[i] != NULL)
	{
		j = 0;
		if (access(p->cmd_args[i][0], F_OK) == 0)
		{
			p->cmd_paths[i] = p->cmd_args[i][0];
			i++;
			continue ;
		}
		// search_valid_cmd_path(p, i);
		while (p->env_paths[j] != NULL)
		{
			p->cmd_paths[i] = ft_strjoin(p->env_paths[j], p->cmd_args[i][0]);
			if (p->cmd_paths[i] == NULL)
				handle_error(p, "Search_valid_cmd_path: ft_strjoin:", 1, 1);
			if (access(p->cmd_paths[i], F_OK) == 0)
				break ;
			free(p->cmd_paths[i]);
			p->cmd_paths[i] = NULL;
			j++;
		}
		if (p->cmd_paths[i] == NULL)
		{
			error_msg = ft_strjoin(p->cmd_args[i][0], ": command not found");
			handle_error(p, error_msg, 0, 0);
			free(error_msg);
		}
		i++;
	}
}


void	get_env_paths(t_pipex *pipex)
{
	int		i;
	char	*defp;

	i = 0;
	defp = "/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin";
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
	add_slash_to_paths(pipex, pipex->env_paths);
}
