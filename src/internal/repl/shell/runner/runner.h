/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:39:11 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/09 03:10:48 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUNNER_H
# define RUNNER_H

# include <signal.h>
# include <stdbool.h>
# include "internal/repl/shell/command/command.h"

typedef enum e_builtin
{
	NOT_BUILTIN,
	BUILTIN_ECHO,
	BUILTIN_EXPORT,
}	t_builtin;


typedef struct s_runner_data
{
	t_arraylist		*pids;
	t_arraylist		*pipes_to_close;
	sig_atomic_t	last_cmd_status;
	t_command		base_cmd;
	t_command		cmd;
}	t_runner_data;

typedef enum e_fork_flag
{
	FORK_NOT,
	FORK_YES
}	t_fork_flag;

int				runner(t_command cmd, int last_status_code);
sig_atomic_t	runner_cmd(t_runner_data *run_data, t_fork_flag should_fork);
t_builtin		runner_maybe_cmd_builtin(t_command cmd);
sig_atomic_t	runner_cmd_builtin(t_builtin builtin, t_command cmd, \
					bool should_exit);
sig_atomic_t	runner_cmd_simple(
					t_runner_data *run_data, t_fork_flag should_fork);
void			runner_cmd_simple_panic(t_runner_data *run_data, char *msg, \
					sig_atomic_t status_code, bool should_exit);
void			runner_cmd_simple_exit_status(
					t_runner_data *runner_data, sig_atomic_t status);
#endif
