/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 01:08:32 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:09:24 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_bonus/repl/shell/command/command.h"
#include "internal_bonus/repl/shell/command/command_internal.h"

int	command_build_is_empty(t_command cmd)
{
	return (cmd->type == CMD_SIMPLE
		&& cmd->simple->cmd_argc == 0
		&& ft_arraylist_len(cmd->io_handlers) == 0);
}

int	command_operator_idx(t_token **tokens, int tokens_len)
{
	int	cmd_operator_idx;
	int	precedence_cur;
	int	precedence_next;
	int	i;
	int	open_parens;

	precedence_cur = command_token_precedence(WORD);
	open_parens = 0;
	cmd_operator_idx = -1;
	i = -1;
	while (++i < tokens_len)
	{
		precedence_next = command_token_precedence(tokens[i]->type);
		if (open_parens == 0 && precedence_next < precedence_cur)
		{
			cmd_operator_idx = i;
			precedence_cur = precedence_next;
		}
		if (tokens[i]->type == OP_PAREN_OPEN)
			open_parens++;
		else if (tokens[i]->type == OP_PAREN_CLOSE)
			open_parens--;
	}
	return (cmd_operator_idx);
}
