/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:06:48 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/23 08:13:20 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_INTERNAL_H
# define SIGNAL_INTERNAL_H

# include <stdbool.h>

void	sigint_handler(int signal, bool redisplay);
void	sigint_handler_redisplay(int signal);
void	sigint_handler_no_redisplay(int signal);

#endif
