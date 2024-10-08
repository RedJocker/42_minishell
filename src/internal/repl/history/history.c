/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 05:57:22 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/08 05:46:29 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <stdbool.h>
#include "ft_ctype.h"
#include "ft_string.h"
#include "internal/default.h"

bool	history_add_input(char *input)
{
	if (!input)
		return (true);
	else if (ft_strncmp(input, "<<", CHAR_BYTE + CHAR_BYTE) == DEFAULT) //TODO: Verify this, "However, it doesn’t have to update the history!", this is in the pdf.
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
