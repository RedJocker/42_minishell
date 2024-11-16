/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:40:01 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/16 04:12:52 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include <signal.h>
# include "internal/repl/shell/command/command.h"

void	expand(t_command cmd, sig_atomic_t last_status_code);

#endif
