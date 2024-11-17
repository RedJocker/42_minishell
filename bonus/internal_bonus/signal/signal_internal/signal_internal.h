/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:06:48 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/02 23:16:57 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_INTERNAL_H
# define SIGNAL_INTERNAL_H

# include <signal.h>

void	signals_initializer_redisplay(struct sigaction *sa_int);
void	signals_initializer_no_redisplay(struct sigaction *sa_int);
void	sigint_handler_redisplay(sig_atomic_t signal);
void	sigint_handler_no_redisplay(sig_atomic_t signal);
int		rl_hook(void);
void	sigint_handler_heredoc(sig_atomic_t signal);

#endif
