/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 13:58:52 by sbalk             #+#    #+#             */
/*   Updated: 2023/08/21 15:10:05 by sbalk            ###   ########.fr       */
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
	free_pipex_struct(pipex);
	write(2, msg, ft_strlen(msg));
	exit(1);
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
	while (i < argc - 2)
	{
		pipex->cmd_args[j] = ft_split(argv[i], ' ');
		if (pipex->cmd_args[j] == NULL)
			error_exit(pipex, ERR_NOMEM);
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

	argc++;
	argv++;
	get_env_paths(&pipex, envp);
	get_cmds(&pipex, argc, argv);
	free_pipex_struct(&pipex);
}
