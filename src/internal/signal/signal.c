/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 00:47:10 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 07:01:26 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include "ft_memlib.h"
#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/repl/shell/shell.h"
#include "internal/signal/signal.h"

static void	sigint_handler(int signal);

void	signals_initializer(void)
{
	struct sigaction	sa_shell;

	ft_memset(&sa_shell, DEFAULT, sizeof(sa_shell));
	sa_shell.sa_flags = SA_RESTART;
	sa_shell.sa_handler = sigint_handler;
	if (sigaction(SIGINT, &sa_shell, NULL) == FAIL
		|| signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		ft_putendl_fd("shell: failed signal: fail to set signals", \
			STDERR_FILENO);
		exit (EXIT_FAILURE);
	}
}

int	signal_status(int value, int type)
{
	static volatile int	signal_status;

	if (type == SET)
		signal_status = value;
	return (signal_status);
}

static void	sigint_handler(int signal)
{
	signal_status(SIG_BASE + signal, SET);
	ft_putstr_fd("\n", STDIN_FILENO);
	rl_on_new_line();
	rl_replace_line("", DEFAULT);
	if (shell_get_redisplay_status())
		rl_redisplay();
}
