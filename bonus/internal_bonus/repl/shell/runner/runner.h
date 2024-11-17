/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:39:11 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:05:00 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUNNER_H
# define RUNNER_H

# include <signal.h>
# include "internal_bonus/repl/shell/command/command.h"

sig_atomic_t	runner(t_command cmd, sig_atomic_t last_cmd_status);

#endif
