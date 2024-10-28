/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/28 02:15:45 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/repl/repl.h"
#include "internal/signal/signal.h"
#include "internal/repl/terminal/terminal.h"
#include "internal/repl/history/history.h"
#include "internal/repl/shell/shell.h"
#include "ft_memlib.h"
#include <readline/readline.h>
#include <readline/history.h>

static void	repl_loop(t_shell *shell);

// void clear_readline_data(void)
// {
//     rl_clear_history();
//     if (rl_line_buffer)
//         rl_free(rl_line_buffer);
//     rl_line_buffer = NULL;
//     rl_done = 1;
// }

int	repl(void)
{
	t_shell	shell;

	ft_bzero(&shell, sizeof(t_shell));
	env_initializer();
	terminal_properties(false);
	while (WAIT)
		repl_loop(&shell);
	env_destroy();
	// rl_clear_history();
	// clear_readline_data();
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
