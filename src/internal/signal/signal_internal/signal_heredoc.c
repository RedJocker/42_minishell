/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:32:08 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/02 23:34:00 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/signal/signal.h"

int	rl_hook(void)
{
	return (DEFAULT);
}

void	sigint_handler_heredoc(sig_atomic_t signal)
{
	signal_status(signal, SET);
	ft_putstr_fd("\n", STDIN_FILENO);
	rl_done = DEFAULT_BEGIN;
	rl_on_new_line();
	rl_replace_line("", DEFAULT);
}
