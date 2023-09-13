/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_two.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 13:54:59 by sbalk             #+#    #+#             */
/*   Updated: 2023/09/13 15:30:17 by sbalk            ###   ########.fr       */
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
