/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 00:47:10 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/30 23:19:09 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include "ft_memlib.h"
#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/signal/signal.h"
#include "internal/signal/signal_internal/signal_internal.h"

static void	signals_initializer_redisplay(struct sigaction *sa_int);
static void	signals_initializer_no_redisplay(struct sigaction *sa_int);

void	signals_initializer(bool redisplay)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, DEFAULT, sizeof(sa_int));
	ft_memset(&sa_quit, DEFAULT, sizeof(sa_quit));
	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = SIG_IGN;
	if (redisplay)
		signals_initializer_redisplay(&sa_int);
	else
		signals_initializer_no_redisplay(&sa_int);
	if (sigaction(SIGINT, &sa_int, NULL) == FAIL
		|| sigaction(SIGQUIT, &sa_quit, NULL) == FAIL)
	{
		ft_putendl_fd("shell: failed signal: fail to set signals", \
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
		ft_putendl_fd("shell: failed signal: fail to set signals", \
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

static void	signals_initializer_redisplay(struct sigaction *sa_int)
{
	sa_int->sa_flags = SA_RESTART;
	sa_int->sa_handler = sigint_handler_redisplay;
}

static void	signals_initializer_no_redisplay(struct sigaction *sa_int)
{
	sa_int->sa_flags = SA_RESTART;
	sa_int->sa_handler = sigint_handler_no_redisplay;
}
