/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:15:50 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 09:46:13 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/default.h"
#include "internal/repl/shell/parse/parse.h"
#include "internal/repl/shell/runner/runner.h"
#include "internal/repl/shell/shell_internal/shell_internal.h"
#include "internal/repl/shell/token/token.h"
#include "internal/signal/signal.h"

t_shell	*shell(void)
{
	static t_shell	shell;

	return (&shell);
}

void	shell_build_token(void)
{
	shell()->str_tokens = parse(shell()->input);
	shell()->tokens = tokens_classify(shell()->str_tokens, \
						&shell()->tokens_len);
}

void	shell_build_command(void)
{
	shell()->command = command_build(shell()->tokens, shell()->tokens_len);
	tokens_destroy(shell()->tokens);
}

void	shell_build_runner(void)
{
	shell()->status = runner(shell()->command, shell()->status);
	signal_status(shell()->status, SET);
	command_destroy(shell()->command);
}
