/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 20:19:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/30 23:49:45 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"
#include "internal/repl/shell/token/token.h"
#include "command_internal.h"
#include "ft_assert.h"

t_command	command_build_simple(
	t_token **tokens, int endtoken_idx)
{
	t_command	cmd;
	char		*err_msg;
	t_token		*err_tkn;
	const char	*err_template = "bash: syntax error near unexpected token `%s'";

	if (command_simple_is_invalid(tokens, &endtoken_idx))
	{
		err_tkn = tokens[endtoken_idx];
		ft_asprintf(&err_msg, err_template, err_tkn->content);
		cmd = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR  << 8);
		free(err_msg);
		return (cmd);
	}
	else
		return (command_simple_new(tokens, endtoken_idx));
}

t_command	command_build_pipe(
	t_token **tokens, int cmd_operator_idx, int tokens_len)
{
	t_command	cmd_before;
	t_command	cmd_after;

	cmd_before = command_build(tokens, cmd_operator_idx);
	if (!cmd_before || cmd_before->type == CMD_INVALID)
		return (cmd_before);
	cmd_after = command_build(
			tokens + cmd_operator_idx + 1, tokens_len - cmd_operator_idx - 1);
	if (!cmd_after || cmd_before->type == CMD_INVALID)
	{
		free(cmd_before);
		return (cmd_after);
	}
	return (command_pipe_new(cmd_before, cmd_after));
}

t_command	command_build(t_token **tokens, int tokens_len)
{
	int	cmd_operator_idx;

	if (tokens_len == 1 && tokens[0]->type == OP_EOF)
		return (command_eof_new());
	cmd_operator_idx = command_operator_idx(tokens, tokens_len);
	if (cmd_operator_idx < 0 || tokens[cmd_operator_idx]->type == OP_NEWLINE)
		return (command_build_simple(tokens, tokens_len));
	else if (tokens[cmd_operator_idx]->type == OP_PIPE)
		return (command_build_pipe(tokens, cmd_operator_idx, tokens_len));
	ft_assert(0, "unexpected execution at command_build");
	return (command_invalid_new("temporarily unnexpected", -1));
}

/*


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
