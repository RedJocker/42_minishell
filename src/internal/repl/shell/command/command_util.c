/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 16:56:15 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/26 21:02:18 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/repl/shell/command/command_internal.h"
#include "internal/repl/shell/token/token.h"

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

void	command_free(t_command cmd)
{
	ft_arraylist_destroy(cmd->io_handlers);
	free(cmd->debug_id);
	free(cmd);
}

int	command_simple_is_invalid(
	t_token **tokens, int *endtoken_idx)
{
	int	i;

	i = -1;
	while (++i < *endtoken_idx)
	{
		if (token_type_is_redirect(tokens[i])
			&& !token_type_is_word(tokens[i + 1]))
		{
			*endtoken_idx = i + 1;
			return (1);
		}
		else if (tokens[i]->type == INVALID)
		{
			*endtoken_idx = i;
			return (1);
		}
	}
	return (0);
}
