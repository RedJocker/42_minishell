/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 00:47:20 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 03:57:03 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H

# include <signal.h>
# include <stdbool.h>
# include "internal_bonus/default.h"

enum e_signal
{
	SIG_BASE = 128,
};

sig_atomic_t	signal_status(sig_atomic_t value, t_operations type);
void			signals_initializer(bool redisplay);
void			sigquit_handler(sig_atomic_t signal);
void			signals_afterfork(void);
void			signals_heredoc(void);

#endif
