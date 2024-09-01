/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 20:19:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/01 16:58:56 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_internal.h"
#include "internal/default.h"
#include "internal/signal/signal.h"
#include "internal/token/token.h"
#include <stdlib.h>
#include <stdio.h>

// TODO: improve invalid message
t_command	command_build_simple(
	t_token **tokens, int fst_token_idx, int endtoken_idx)
{
	if (command_simple_is_invalid(tokens, fst_token_idx, &endtoken_idx))
		return (command_invalid_new(
				"bash: syntax error near unexpected token `>'"));
	else
		return (command_simple_new(tokens, fst_token_idx, endtoken_idx));
}

t_command	command_build(t_token **tokens, int tokens_len)
{
	int	cmd_operator_idx;

	// TODO handle with private builtin to exit printing exit
	if (tokens_len == 1 && tokens[0]->type == OP_EOF)
	{
		tokens_destroy(tokens);
		printf("exit\n");
		exit(signal_status(DEFAULT, GET));
	}
	cmd_operator_idx = command_operator_idx(tokens, tokens_len);
	if (tokens[cmd_operator_idx]->type == OP_NEWLINE)
		return (command_build_simple(tokens, 0, tokens_len));
	return (command_invalid_new("temporarily unnexpected"));
}

/*

t_command	command_build_pipe()
{
	cmd_before = command_build(tokens, cmd_operator_idx);
	if (!cmd_before || invalid(cmd_before))
		return (cmd_before);
	cmd_after = command_build(
		tokens + cmd_operator_idx, tokens_len - cmd_operator_idx);
	if (!cmd_after || invalid(cmd_after))
	{
		free(cmd_before);
		return (cmd_after);
	}
	return pipe_new(cmd_before, cmd_after);
}

Simple()
Pipe()
And()
Or()
Group()


ls
Simple(ls)

ls | cat -e
Pipe(Simple(ls), Simple(cat -e))

ls | cat -e | wc
(Pipe(ls, Pipe(cat -e, w)))


ls | wc && echo yes | wc
And(Pipe(ls, wc), Pipe(echo yes, wc))
*/
