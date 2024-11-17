/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:00:44 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include "ft_memlib.h"
#include "internal_bonus/default.h"
#include "internal_bonus/env/env.h"
#include "internal_bonus/repl/repl.h"
#include "internal_bonus/signal/signal.h"
#include "internal_bonus/repl/terminal/terminal.h"
#include "internal_bonus/repl/history/history.h"
#include "internal_bonus/repl/shell/shell.h"

static void	repl_loop(t_shell *shell);

int	repl(void)
{
	t_shell	shell;

	ft_bzero(&shell, sizeof(t_shell));
	env_initializer();
	terminal_properties(false);
	while (WAIT)
		repl_loop(&shell);
	env_destroy();
	return (signal_status(DEFAULT, GET));
}

static void	repl_loop(t_shell *shell)
{
	signal_status(DEFAULT, SET);
	shell_set_input(shell);
	if (!history_add_input(shell->input))
		return ;
	shell_command(shell);
	terminal_properties(true);
}
