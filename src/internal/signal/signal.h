/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 00:47:20 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/23 08:50:57 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H

# include <signal.h>
# include <stdbool.h>
# include "internal/default.h"

enum e_signal
{
	SIG_BASE = 128,
};

sig_atomic_t	signal_status(sig_atomic_t value, t_operations type);
void			signals_initializer(bool redisplay);

#endif
