/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 15:42:58 by sbalk             #+#    #+#             */
/*   Updated: 2023/08/22 16:24:09 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX
# define PIPEX

# include "libft.h"
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <sys/wait.h>

#define ERR_NOMEM	"Could not allocate memory!"
#define ERR_CMD		"Command not found"


typedef enum e_bool
{
	false,
	true
} t_bool;

typedef struct s_pipex
{
	int		in_fd;
	int		out_fd;
	t_bool	here_doc;
	t_bool	is_invalid_infile;
	char	*infile;
	char	*outfile;
	char	**env_paths;
	char	***cmd_args;
	int		cmd_count;
	char	**envp;
}			t_pipex;

/* String functions */


/* Error handling */


/* index functions */


/* Sorting functions */


/* List functions */


/* Push swap functions */



#endif