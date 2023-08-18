/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 13:58:52 by sbalk             #+#    #+#             */
/*   Updated: 2023/08/18 15:30:57 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

static char	**get_env_paths(char **envp)
{
	int		i;
	char	**ret;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			ret = ft_split(envp[i] + 5, ':');
			return (ret);
		}
		i++;
	}
	return (NULL);
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

	char	**envpath;
	argc++;
	argv++;

	envpath = get_env_paths(envp);
	if (!envpath)
		ft_printf("NULL returned\n");
	while (*envpath != NULL)
	{
		ft_printf("%s\n", *envpath);
		envpath++;
	}
}
