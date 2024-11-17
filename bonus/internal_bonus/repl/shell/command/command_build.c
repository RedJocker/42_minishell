/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 20:19:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:10:07 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_assert.h"
#include "internal_bonus/repl/shell/command/command.h"
#include "internal_bonus/repl/shell/command/command_internal.h"
#include "internal_bonus/repl/shell/token/token.h"

t_command	command_build_simple(t_token **tokens, int endtoken_idx)
{
	if (command_simple_is_invalid(tokens, &endtoken_idx))
		return (command_build_panic_zero(tokens[endtoken_idx]->content));
	else
		return (command_simple_new(tokens, endtoken_idx));
}

t_command	command_build_pipe(t_token **tokens, int op_idx, int tokens_len)
{
	t_command	cmd_before;
	t_command	cmd_after;

	cmd_before = command_build(tokens, op_idx);
	if (!cmd_before || cmd_before->type == CMD_INVALID)
		return (cmd_before);
	else if (command_build_is_empty(cmd_before))
		return (command_build_panic_one(cmd_before, tokens[op_idx]->content));
	cmd_after = command_build(
			tokens + op_idx + 1, tokens_len - op_idx - 1);
	if (!cmd_after || cmd_after->type == CMD_INVALID)
	{
		command_destroy(cmd_before);
		return (cmd_after);
	}
	else if (command_build_is_empty(cmd_after))
		return (command_build_panic_two(cmd_before,
				cmd_after, tokens[op_idx + 1]->content));
	return (command_pipe_new(cmd_before, cmd_after));
}

t_command	command_build_and(t_token **tokens, int op_idx, int tokens_len)
{
	t_command	cmd_before;
	t_command	cmd_after;

	cmd_before = command_build(tokens, op_idx);
	if (!cmd_before || cmd_before->type == CMD_INVALID)
		return (cmd_before);
	else if (command_build_is_empty(cmd_before))
		return (command_build_panic_one(cmd_before, tokens[op_idx]->content));
	cmd_after = command_build(
			tokens + op_idx + 1, tokens_len - op_idx - 1);
	if (!cmd_after || cmd_after->type == CMD_INVALID)
	{
		free(cmd_before);
		return (cmd_after);
	}
	else if (command_build_is_empty(cmd_after))
		return (command_build_panic_two(cmd_before,
				cmd_after, tokens[op_idx + 1]->content));
	return (command_and_new(cmd_before, cmd_after));
}

t_command	command_build_or(t_token **tokens, int op_idx, int tokens_len)
{
	t_command	cmd_before;
	t_command	cmd_after;

	cmd_before = command_build(tokens, op_idx);
	if (!cmd_before || cmd_before->type == CMD_INVALID)
		return (cmd_before);
	else if (command_build_is_empty(cmd_before))
		return (command_build_panic_one(cmd_before, tokens[op_idx]->content));
	cmd_after = command_build(
			tokens + op_idx + 1, tokens_len - op_idx - 1);
	if (!cmd_after || cmd_after->type == CMD_INVALID)
	{
		free(cmd_before);
		return (cmd_after);
	}
	else if (command_build_is_empty(cmd_after))
		return (command_build_panic_two(cmd_before,
				cmd_after, tokens[op_idx + 1]->content));
	return (command_or_new(cmd_before, cmd_after));
}

t_command	command_build(t_token **tokens, int tokens_len)
{
	int	cmd_operator_idx;

	if (tokens_len == 1 && tokens[0]->type == OP_EOF)
		return (command_eof_new());
	cmd_operator_idx = command_operator_idx(tokens, tokens_len);
	if (cmd_operator_idx < 0)
		return (command_build_simple(tokens, tokens_len + 1));
	else if (tokens[cmd_operator_idx]->type == OP_NEWLINE)
		return (command_build_simple(tokens, tokens_len));
	else if (tokens[cmd_operator_idx]->type == OP_PIPE)
		return (command_build_pipe(tokens, cmd_operator_idx, tokens_len));
	else if (tokens[cmd_operator_idx]->type == OP_AND)
		return (command_build_and(tokens, cmd_operator_idx, tokens_len));
	else if (tokens[cmd_operator_idx]->type == OP_OR)
		return (command_build_or(tokens, cmd_operator_idx, tokens_len));
	else if (tokens[cmd_operator_idx]->type == OP_PAREN_OPEN)
		return (command_build_parentheses(tokens, 0, tokens_len));
	else if (tokens[cmd_operator_idx]->type == OP_PAREN_CLOSE)
		return (command_build_paren_err(tokens, cmd_operator_idx, tokens_len));
	ft_assert(0, "unexpected execution at command_build");
	return (command_invalid_new("temporarily unnexpected", -1));
}
