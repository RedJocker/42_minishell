/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 05:57:22 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 06:10:25 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <stdbool.h>
#include "ft_ctype.h"

bool	history_add_input(char *input)
{
	if (!input)
		return (true);
	while (*input)
	{
		if (!ft_isspace(*input))
		{
			add_history(input);
			return (true);
		}
		input++;
	}
	return (false);
}
