/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build_paren.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 22:05:18 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/19 22:13:21 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_assert.h"
#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"
#include "internal/repl/shell/token/token.h"

static const char	*g_err_template = "minishell: syntax error "\
							"near unexpected token `%s'";

static int	find_paren_end(t_token **tokens, int tokens_len)
{
	int	open_paren;
	int	i;

	ft_assert(tokens[0]->type == OP_PAREN_OPEN, "expected paren open");
	i = 1;
	open_paren = 1;
	while (i < tokens_len)
	{
		if (tokens[i]->type == OP_PAREN_OPEN)
			open_paren++;
		else if (tokens[i]->type == OP_PAREN_CLOSE)
			open_paren--;
		if (open_paren == 0)
			break ;
		else if (token_type_is_either_nl_eof(tokens[i]))
			break ;
		i++;
	}
	return (i);
}

int	has_redirect_before(t_token **tokens, int op_idx)
{
	int	i;

	i = -1;
	while (++i < op_idx)
	{
		if (token_type_is_redirect(tokens[i]))
			return (1);
	}
	return (0);
}

t_command	command_build_parentheses(
	t_token **tokens, int op_idx, int tokens_len)
{
	t_command	cmd;
	int			close_i;

	if (tokens[0]->type != OP_PAREN_OPEN && has_redirect_before(tokens, op_idx))
		return (command_build_panic_zero(tokens[op_idx]->content));
	else if (tokens[0]->type != OP_PAREN_OPEN)
		return (command_build_panic_zero(tokens[op_idx + 1]->content));
	close_i = find_paren_end(tokens, tokens_len);
	if (tokens[close_i]->type != OP_PAREN_CLOSE \
		|| !token_type_is_end_op(tokens[close_i + 1]) \
		|| close_i < 2)
	{
		close_i += (close_i + 1 < tokens_len \
								&& close_i > 1 \
								&& !token_type_is_end_op(tokens[close_i + 1]));
		return (command_build_panic_zero(tokens[close_i]->content));
	}
	cmd = command_build(tokens + 1, close_i - 1);
	if (!cmd || cmd->type == CMD_INVALID)
		return (cmd);
	else if (command_build_is_empty(cmd))
		return (command_build_panic_one(cmd, \
			tokens[close_i]->content));
	return (command_paren_new(cmd));
}

t_command	command_build_paren_err(
	t_token **tokens, int cmd_operator_idx, int tokens_len)
{
	t_command	cmd;
	char		*err_msg;

	(void) tokens_len;
	ft_asprintf(&err_msg, g_err_template, tokens[cmd_operator_idx]->content);
	cmd = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR);
	free(err_msg);
	return (cmd);
}
