/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_2darray.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:07:40 by sbalk             #+#    #+#             */
/*   Updated: 2023/08/07 14:14:20 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_2darray(void **arr, size_t size)
{
	if (arr == NULL)
		return ;
	while (size--)
		if (arr[size] != NULL)
			free(arr[size]);
	free (arr);
}
