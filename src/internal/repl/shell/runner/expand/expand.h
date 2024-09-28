/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:40:01 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/25 00:23:33 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "internal/repl/shell/command/command.h"
# include "internal/repl/shell/runner/expand/expand_internal.h"
# include <signal.h>

void	expand(t_command cmd, sig_atomic_t last_status_code);

#endif
