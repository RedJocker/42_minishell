/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 06:34:42 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:01:26 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include "internal_bonus/repl/shell/parse/parse.h"
#include "internal_bonus/repl/shell/runner/runner.h"
#include "internal_bonus/repl/shell/shell.h"
#include "internal_bonus/repl/shell/token/token.h"
#include "internal_bonus/signal/signal.h"
#include "internal_bonus/env/env.h"

void	shell_set_input(t_shell *shell)
{
	char	*ps1;

	ps1 = env_get_value("PS1");
	signals_initializer(true);
	shell->input = readline(ps1);
	free(ps1);
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
}