/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 20:19:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/25 21:56:37 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_internal.h"

// TODO: improve invalid message
t_command	command_build_simple(
	t_token **tokens, int fst_token_idx, int endtoken_idx)
{
	if (command_simple_is_invalid(tokens, fst_token_idx, &endtoken_idx))
		return (command_invalid_new(
				"bash: syntax error near unexpected token `newline'"));
	else
		return (command_simple_new(tokens, fst_token_idx, endtoken_idx));
}

t_command	command_build(t_token **tokens, int tokens_len)
{
	int			cmd_operator_idx;

	cmd_operator_idx = command_operator_idx(tokens, tokens_len);
	if (cmd_operator_idx == -1)
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
