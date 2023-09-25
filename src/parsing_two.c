/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_two.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 13:54:59 by sbalk             #+#    #+#             */
/*   Updated: 2023/09/25 10:36:54 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*get_quote_command(t_pipex *pipex, char **str)
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
		handle_error(pipex, "calloc: parse quote: ", 1, 1);
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

static void	search_valid_cmd_path(t_pipex *p, int i)
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
	int		i;
	char	*error_msg;

	i = 0;
	p->cmd_paths = ft_calloc(sizeof(char *), p->command_count + 1);
	if (p->cmd_paths == NULL)
		handle_error(p, "get_cmd_paths: calloc:", 1, 1);
	while (p->cmd_args[i] != NULL)
	{
		if (access(p->cmd_args[i][0], F_OK) == 0)
		{
			p->cmd_paths[i] = p->cmd_args[i][0];
			i++;
			continue ;
		}
		search_valid_cmd_path(p, i);
		if (p->cmd_paths[i] == NULL)
		{
			error_msg = ft_strjoin(p->cmd_args[i][0], ": command not found");
			handle_error(p, error_msg, 0, 0);
			free(error_msg);
		}
		i++;
	}
}
