/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 20:19:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/19 16:59:14 by maurodri         ###   ########.fr       */
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
		cmd = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR);
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
	t_command   cmd_after;
	char		*err_msg;
	const char	*err_template = "bash: syntax error near unexpected token `%s'";

	cmd_before = command_build(tokens, cmd_operator_idx);
	if (!cmd_before || cmd_before->type == CMD_INVALID)
		return (cmd_before);
	else if (cmd_before->type == CMD_SIMPLE && cmd_before->simple->cmd_argc == 0)
	{
		command_destroy(cmd_before);
		ft_asprintf(&err_msg, err_template, tokens[cmd_operator_idx]->content);
		cmd_before = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR);
		free(err_msg);
		return (cmd_before);
	}
	cmd_after = command_build(
			tokens + cmd_operator_idx + 1, tokens_len - cmd_operator_idx - 1);
	if (!cmd_after || cmd_after->type == CMD_INVALID)
	{
		command_destroy(cmd_before);
		return (cmd_after);
	}
	else if (cmd_before->type == CMD_SIMPLE && cmd_after->simple->cmd_argc == 0)
	{
		command_destroy(cmd_after);
		command_destroy(cmd_before);
		ft_asprintf(&err_msg, err_template, tokens[cmd_operator_idx + 1]->content);
		cmd_before = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR);
		free(err_msg);
		return (cmd_before);
	}
	return (command_pipe_new(cmd_before, cmd_after));
}

t_command	command_build_and(
	t_token **tokens, int cmd_operator_idx, int tokens_len)
{
	t_command	cmd_before;
	t_command   cmd_after;
	char		*err_msg;
	const char	*err_template = "bash: syntax error near unexpected token `%s'";

	cmd_before = command_build(tokens, cmd_operator_idx);
	if (!cmd_before || cmd_before->type == CMD_INVALID)
		return (cmd_before);
	else if (cmd_before->type == CMD_SIMPLE && cmd_before->simple->cmd_argc == 0)
	{
		command_destroy(cmd_before);
		ft_asprintf(&err_msg, err_template, tokens[cmd_operator_idx]->content);
		cmd_before = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR);
		free(err_msg);
		return (cmd_before);
	}
	cmd_after = command_build(
			tokens + cmd_operator_idx + 1, tokens_len - cmd_operator_idx - 1);
	if (!cmd_after || cmd_before->type == CMD_INVALID)
	{
		free(cmd_before);
		return (cmd_after);
	}
	else if (cmd_before->type == CMD_SIMPLE && cmd_after->simple->cmd_argc == 0)
	{
		command_destroy(cmd_after);
		command_destroy(cmd_before);
		ft_asprintf(&err_msg, err_template, tokens[cmd_operator_idx + 1]->content);
		cmd_before = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR);
		free(err_msg);
		return (cmd_before);
	}
	return (command_and_new(cmd_before, cmd_after));
}


t_command	command_build_or(
	t_token **tokens, int cmd_operator_idx, int tokens_len)
{
	t_command	cmd_before;
	t_command   cmd_after;
	char		*err_msg;
	const char	*err_template = "bash: syntax error near unexpected token `%s'";

	cmd_before = command_build(tokens, cmd_operator_idx);
	if (!cmd_before || cmd_before->type == CMD_INVALID)
		return (cmd_before);
	else if (cmd_before->type == CMD_SIMPLE && cmd_before->simple->cmd_argc == 0)
	{
		command_destroy(cmd_before);
		ft_asprintf(&err_msg, err_template, tokens[cmd_operator_idx]->content);
		cmd_before = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR);
		free(err_msg);
		return (cmd_before);
	}
	cmd_after = command_build(
			tokens + cmd_operator_idx + 1, tokens_len - cmd_operator_idx - 1);
	if (!cmd_after || cmd_before->type == CMD_INVALID)
	{
		free(cmd_before);
		return (cmd_after);
	}
	else if (cmd_before->type == CMD_SIMPLE && cmd_after->simple->cmd_argc == 0)
	{
		command_destroy(cmd_after);
		command_destroy(cmd_before);
		ft_asprintf(&err_msg, err_template, tokens[cmd_operator_idx + 1]->content);
		cmd_before = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR);
		free(err_msg);
		return (cmd_before);
	}
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
