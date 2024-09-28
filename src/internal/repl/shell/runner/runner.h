/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:39:11 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/28 00:10:24 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUNNER_H
# define RUNNER_H

# include "internal/repl/shell/command/command.h"
# include <signal.h>

typedef enum e_builtin
{
	NOT_BUILTIN,
	BUILTIN_ECHO,
	BUILTIN_EXPORT,
}	t_builtin;

int				runner(t_command cmd, int last_status_code);
int				runner_cmd(
					t_command cmd, t_arraylist *pids, int last_cmd_status);
t_builtin		runner_maybe_cmd_builtin(t_command cmd);
sig_atomic_t	runner_cmd_builtin(
					t_builtin builtin, t_command cmd, t_arraylist *pids);
sig_atomic_t	runner_cmd_simple(t_command cmd, t_arraylist *pids);

#endif
