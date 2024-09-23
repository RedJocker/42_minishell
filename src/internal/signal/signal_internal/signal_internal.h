/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:06:48 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/23 10:55:00 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_INTERNAL_H
# define SIGNAL_INTERNAL_H

# include <signal.h>

void	sigint_handler_redisplay(sig_atomic_t signal);
void	sigint_handler_no_redisplay(sig_atomic_t signal);

#endif
