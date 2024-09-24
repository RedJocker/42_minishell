/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 06:34:42 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/24 20:17:42 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include "internal/default.h"
#include "internal/repl/shell/parse/parse.h"
#include "internal/repl/shell/runner/runner.h"
#include "internal/repl/shell/shell.h"
#include "internal/repl/shell/token/token.h"
#include "internal/signal/signal.h"

void	shell_set_input(t_shell *shell)
{
	signals_initializer(true);
	shell->input = readline("RedWillShell$ ");
	signals_initializer(false);
}

void	shell_command(t_shell *shell)
{
	shell->str_tokens = parse_input(shell->input);
	shell->tokens = tokens_classify(shell->str_tokens, \
						&shell->tokens_len);
	shell->command = command_build(shell->tokens, shell->tokens_len);
	tokens_destroy(shell->tokens);
	shell->status = runner(shell->command, shell->status);
	signal_status(shell->status, SET);
	command_destroy(shell->command);
}
