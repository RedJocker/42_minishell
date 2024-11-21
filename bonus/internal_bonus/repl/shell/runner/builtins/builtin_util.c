/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:43:15 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/20 22:03:32 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ctype.h"

int	is_identifier(char *maybe_identifier)
{
	if (!ft_isalpha(*maybe_identifier) && *maybe_identifier != '_')
		return (0);
	while (*(++maybe_identifier))
	{
		if (*maybe_identifier == '=')
			return (*(maybe_identifier + 1) == '\0');
		if (!ft_isalnum(*maybe_identifier) && *maybe_identifier != '_')
			return (0);
	}
	return (1);
}
