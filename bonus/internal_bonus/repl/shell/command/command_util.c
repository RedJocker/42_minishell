/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 16:56:15 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:08:29 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_bonus/repl/shell/command/command_internal.h"
#include "internal_bonus/repl/shell/token/token.h"

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
	else if (token_type == OP_PAREN_CLOSE)
		return (7);
	else if (token_type == OP_PAREN_OPEN)
		return (6);
	else if (token_type == OP_PIPE)
		return (5);
	else if (token_type == OP_AND || token_type == OP_OR)
		return (4);
	return (11);
}
