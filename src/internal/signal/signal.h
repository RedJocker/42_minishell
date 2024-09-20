/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 00:47:20 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 21:16:28 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H

enum e_signal
{
	SIG_BASE = 128,
};

int		signal_status(int value, int type);
void	signals_initializer(int should_redisplay);

#endif
