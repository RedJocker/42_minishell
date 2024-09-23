/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/23 10:14:03 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/repl/repl.h"
#include "internal/signal/signal.h"
#include "internal/repl/terminal/terminal.h"
#include "internal/repl/history/history.h"
#include "internal/repl/shell/shell.h"

static void	repl_loop(t_shell *shell);

int	repl(void)
{
	env_initializer();
	terminal_properties(false);
	while (WAIT)
		repl_loop(shell());
	env_destroy();
	return (signal_status(DEFAULT, GET));
}

static void	repl_loop(t_shell *shell)
{
	shell_set_input(shell);
	if (!history_add_input(shell->input))
		return ;
	shell_command(shell);
	terminal_properties(true);
}
