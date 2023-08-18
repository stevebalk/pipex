/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalk <sbalk@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 15:56:28 by sbalk             #+#    #+#             */
/*   Updated: 2023/05/06 13:15:09 by sbalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_length;
	size_t	s2_length;
	char	*new_string;

	s1_length = ft_strlen(s1);
	s2_length = ft_strlen(s2);
	new_string = malloc((s1_length + s2_length + 1) * sizeof(char));
	if (new_string == NULL)
		return (NULL);
	ft_strlcpy(new_string, s1, s1_length + 1);
	ft_strlcpy(new_string + s1_length, s2, s2_length + 1);
	return (new_string);
}
