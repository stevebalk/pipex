/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 15:42:58 by sbalk             #+#    #+#             */
/*   Updated: 2023/08/18 14:41:21 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX
# define PIPEX

# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>

typedef enum e_bool
{
	false,
	true
} t_bool;

typedef struct s_pipex
{
	int in_fd;
	int out_fd;
	t_bool here_doc;
	t_bool is_invalid_infile;
	char *infile;
	char *outfile;
	char **cmd_paths;
	char ***cmd_args;
	int cmd_count;
} t_pipex;

/* String functions */


/* Error handling */


/* index functions */


/* Sorting functions */


/* List functions */


/* Push swap functions */



#endif