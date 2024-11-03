/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:06:48 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/31 01:51:42 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_INTERNAL_H
# define SIGNAL_INTERNAL_H

# include <signal.h>

void	sigint_handler_redisplay(sig_atomic_t signal);
void	sigint_handler_no_redisplay(sig_atomic_t signal);
void	sigint_handler_heredoc(sig_atomic_t signal);

#endif
