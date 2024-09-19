/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 09:26:00 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/repl/repl.h"
#include "internal/signal/signal.h"
#include "internal/repl/terminal/terminal.h"
#include "internal/repl/history/history.h"
#include "internal/repl/shell/shell.h"

static void	repl_loop(void);

int	repl(void)
{
	signals_initializer();
	env_initializer();
	terminal_properties(false);
	while (WAIT)
		repl_loop();
	env_destroy();
	return (signal_status(DEFAULT, GET));
}

static void	repl_loop(void)
{
	shell_set_input();
	if (!history_add_input(shell_get_input()))
		return ;
	shell_command();
	terminal_properties(true);
}
