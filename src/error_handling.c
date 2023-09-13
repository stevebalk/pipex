/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 13:47:34 by sbalk             #+#    #+#             */
/*   Updated: 2023/09/08 13:47:47 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
