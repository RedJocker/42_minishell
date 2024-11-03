/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_initializer.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:10:50 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/02 23:12:02 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "internal/signal/signal_internal/signal_internal.h"

void	signals_initializer_redisplay(struct sigaction *sa_int)
{
	sa_int->sa_flags = SA_RESTART;
	sa_int->sa_handler = sigint_handler_redisplay;
}

void	signals_initializer_no_redisplay(struct sigaction *sa_int)
{
	sa_int->sa_flags = SA_RESTART;
	sa_int->sa_handler = sigint_handler_no_redisplay;
}
