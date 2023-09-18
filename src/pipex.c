/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 13:58:52 by sbalk             #+#    #+#             */
/*   Updated: 2023/09/18 16:58:12 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	free_pipex_struct(t_pipex *pipex)
{
	ft_free_2darray((void ***) pipex->cmd_args);
	ft_free_array((void **) pipex->env_paths);
	ft_free_array((void **) pipex->cmd_paths);
}

static void	init_struct(int *argc, char **argv[], t_pipex *pipex, char *envp[])
{
	pipex->envp = envp;
	pipex->prog_name = "pipex";
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
	init_struct(&argc, &argv, &pipex, envp);
	get_env_paths(&pipex);
	get_cmds(&pipex, argc, argv);
	get_cmd_paths(&pipex);
	execute(&pipex);
	free_pipex_struct(&pipex);
}
