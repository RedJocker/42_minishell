/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 03:17:12 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 04:07:04 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUNNER_INTERNAL_H
# define RUNNER_INTERNAL_H

# include <signal.h>
# include <stdbool.h>
# include "internal/repl/shell/command/command.h"
# include <signal.h>

typedef struct s_runner_data
{
	t_arraylist		pids;
	t_arraylist		pipes_to_close;
	t_arraylist		backup_pids;
	sig_atomic_t	last_cmd_status;
	t_command		base_cmd;
	t_command		cmd;
}	t_runner_data;

typedef enum e_fork_flag
{
	FORK_NOT,
	FORK_YES
}	t_fork_flag;

sig_atomic_t	runner_cmd(t_runner_data *run_data, t_fork_flag should_fork);
sig_atomic_t	runner_cmd_simple(
					t_runner_data *run_data, t_fork_flag should_fork);
sig_atomic_t	runner_cmd_paren(t_runner_data *run_data);
sig_atomic_t	runner_cmd_or(t_runner_data *run_data);
sig_atomic_t	runner_cmd_and(t_runner_data *run_data);
sig_atomic_t	runner_cmd_pipe(t_runner_data *run_data);
sig_atomic_t	runner_cmd_invalid(t_command cmd, t_runner_data *runner_data);
void			runner_cmd_eof(t_runner_data *run_data);
//
void			runner_cmd_simple_panic(t_runner_data *run_data, char *msg, \
					sig_atomic_t status_code, bool should_exit);
void			runner_cmd_simple_exit_status(
					t_runner_data *runner_data, sig_atomic_t status);
void			runner_data_init(
					t_runner_data *run_data,
					t_command cmd,
					sig_atomic_t last_cmd_status);
void			runner_data_clean(t_runner_data *run_data);
void			runner_heredoc_prompt(t_command cmd);
void			runner_heredoc_to_fd(t_command cmd);
sig_atomic_t	runner_cmd_simple_execve(t_runner_data *run_data);
void			close_fd_lst(t_arraylist fd_lst);
sig_atomic_t	runner_exit_signal(sig_atomic_t	status);
void			runner_wait(t_runner_data *run_data, sig_atomic_t *status);
sig_atomic_t	runner_normalize_status(sig_atomic_t status);

#endif
