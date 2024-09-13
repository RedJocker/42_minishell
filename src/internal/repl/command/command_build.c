/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 20:19:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/13 00:42:36 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/repl/command/command_internal.h"
#include "internal/signal/signal.h"
#include "internal/token/token.h"

t_command	command_build_simple(
	t_token **tokens, int fst_token_idx, int endtoken_idx)
{
	t_command	cmd;
	char		*err_msg;
	t_token		*err_tkn;
	const char	*err_template = "bash: syntax error near unexpected token `%s'";

	if (command_simple_is_invalid(tokens, fst_token_idx, &endtoken_idx))
	{
		err_tkn = tokens[endtoken_idx];
		ft_asprintf(&err_msg, err_template, err_tkn->content);
		cmd = command_invalid_new(err_msg);
		free(err_msg);
		return (cmd);
	}
	else
		return (command_simple_new(tokens, fst_token_idx, endtoken_idx));
}

t_command	command_build(t_token **tokens, int tokens_len)
{
	int	cmd_operator_idx;

	// TODO: handle with private builtin to exit printing exit
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
