/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 16:56:15 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/20 01:33:51 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/repl/shell/command/command_internal.h"

int	command_debug_id(void)
{
	static int	id = 0;

	return (id++);
}

int	command_token_precedence(t_token_type token_type)
{
	if (token_type == WORD)
		return (10);
	else if (token_type == OP_EOF)
		return (9);
	else if (token_type == OP_NEWLINE)
		return (8);
	else if (token_type == OP_PIPE)
		return (7);
	return (11);
}
