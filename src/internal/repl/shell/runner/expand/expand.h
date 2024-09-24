/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:40:01 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/24 20:46:25 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "internal/repl/shell/command/command.h"
# include <signal.h>

void	runner_cmd_expand(t_command cmd, sig_atomic_t last_status_code);

#endif
