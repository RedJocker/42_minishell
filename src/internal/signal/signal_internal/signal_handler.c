/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:13:28 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/23 10:54:38 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/signal/signal.h"

static void	sigint_handler(sig_atomic_t signal, bool redisplay);

void	sigint_handler_redisplay(int signal)
{
	bool	redisplay;

	redisplay = true;
	sigint_handler(signal, redisplay);
}

void	sigint_handler_no_redisplay(sig_atomic_t signal)
{
	bool	redisplay;

	redisplay = false;
	sigint_handler(signal, redisplay);
}

static void	sigint_handler(sig_atomic_t signal, bool redisplay)
{
	signal_status(SIG_BASE + signal, SET);
	ft_putstr_fd("\n", STDIN_FILENO);
	rl_on_new_line();
	rl_replace_line("", DEFAULT);
	if (redisplay)
		rl_redisplay();
}
