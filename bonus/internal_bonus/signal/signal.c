/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 00:47:10 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 03:59:10 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include "ft_memlib.h"
#include "ft_stdio.h"
#include "internal_bonus/default.h"
#include "internal_bonus/signal/signal.h"
#include "internal_bonus/signal/signal_internal/signal_internal.h"

void	signals_initializer(bool redisplay)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, DEFAULT, sizeof(sa_int));
	ft_memset(&sa_quit, DEFAULT, sizeof(sa_quit));
	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = SIG_IGN;
	rl_event_hook = NULL;
	if (redisplay)
		signals_initializer_redisplay(&sa_int);
	else
		signals_initializer_no_redisplay(&sa_int);
	if (sigaction(SIGINT, &sa_int, NULL) == FAIL
		|| sigaction(SIGQUIT, &sa_quit, NULL) == FAIL)
	{
		ft_putendl_fd("minishell: failed signal: fail to set signals", \
			STDERR_FILENO);
		exit (EXIT_FAILURE);
	}
}

void	signals_heredoc(void)
{
	struct sigaction	sa_int;

	ft_memset(&sa_int, DEFAULT, sizeof(sa_int));
	sa_int.sa_handler = sigint_handler_heredoc;
	if (isatty(STDIN_FILENO))
		rl_event_hook = rl_hook;
	if (sigaction(SIGINT, &sa_int, NULL) == FAIL)
	{
		ft_putendl_fd("minishell: failed signal: fail to set signals", \
			STDERR_FILENO);
		exit (EXIT_FAILURE);
	}
}

void	signals_afterfork(void)
{
	struct sigaction	sa_quit;

	ft_memset(&sa_quit, DEFAULT, sizeof(sa_quit));
	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = sigquit_handler;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == FAIL)
	{
		ft_putendl_fd("minishell: failed signal: fail to set signals", \
			STDERR_FILENO);
		exit (EXIT_FAILURE);
	}
}

sig_atomic_t	signal_status(sig_atomic_t value, t_operations type)
{
	static volatile sig_atomic_t	signal_status;

	if (type == SET)
		signal_status = value;
	return (signal_status);
}
