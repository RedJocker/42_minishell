/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 03:55:15 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:21:20 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include "ft_assert.h"
#include "internal_bonus/default.h"
#include "internal_bonus/repl/shell/runner/runner_internal.h"

sig_atomic_t	runner_cmd_pipe(t_runner_data *run_data)
{
	sig_atomic_t	status;
	int				pipe_fds[2];
	const t_command	cmd = run_data->cmd;
	int				*to_close;

	ft_assert(cmd->type == CMD_PIPE, "expected only cmd_pipe");
	status = EXIT_OK;
	pipe(pipe_fds);
	command_add_pipe_io(cmd->pipe->cmd_before, pipe_fds[1], IO_OUT);
	command_add_pipe_io(cmd->pipe->cmd_after, pipe_fds[0], IO_IN);
	to_close = malloc(sizeof(int));
	*to_close = pipe_fds[0];
	run_data->pipes_to_close = ft_arraylist_add(run_data->pipes_to_close, \
								to_close);
	run_data->cmd = cmd->pipe->cmd_before;
	runner_cmd(run_data, FORK_YES);
	run_data->cmd = cmd->pipe->cmd_after;
	runner_cmd(run_data, FORK_YES);
	return (status);
}
