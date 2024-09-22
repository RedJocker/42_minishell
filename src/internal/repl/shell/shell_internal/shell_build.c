/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:15:50 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/21 17:29:44 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/default.h"
#include "internal/repl/shell/parse/parse.h"
#include "internal/repl/shell/runner/runner.h"
#include "internal/repl/shell/shell_internal/shell_internal.h"
#include "internal/repl/shell/token/token.h"
#include "internal/signal/signal.h"
#include "internal/repl/shell/shell.h"

void	shell_build_token(t_shell *shell)
{
	shell->str_tokens = parse(shell->input);
	shell->tokens = tokens_classify(shell->str_tokens, \
						&shell->tokens_len);
}

void	shell_build_command(t_shell *shell)
{
	shell->command = command_build(shell->tokens, shell->tokens_len);
	tokens_destroy(shell->tokens);
}
