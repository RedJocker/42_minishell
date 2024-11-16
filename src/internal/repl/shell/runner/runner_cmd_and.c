/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_and.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 03:27:40 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 23:01:21 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "ft_assert.h"
#include "ft_util.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/runner/runner_internal.h"

sig_atomic_t	runner_cmd_and(t_runner_data *run_data)
{
	sig_atomic_t	status;
	const t_command	cmd = run_data->cmd;

	ft_assert(cmd->type == CMD_AND, "expected only cmd_and");
	run_data->backup_pids = ft_arraylist_add(\
								run_data->backup_pids, run_data->pids);
	run_data->pids = ft_arraylist_new((t_consumer) free);
	run_data->cmd = cmd->and->cmd_before;
	status = runner_cmd(run_data, FORK_NOT);
	status = status << 8;
	runner_wait(run_data, &status);
	ft_arraylist_destroy(run_data->pids);
	run_data->pids = ft_arraylist_pop(run_data->backup_pids);
	if (WIFSIGNALED(status))
		return (runner_exit_signal(status));
	if (WIFEXITED(status))
		run_data->last_cmd_status = (WEXITSTATUS(status));
	if (run_data->last_cmd_status != 0)
	{
		command_close_ios(cmd->and->cmd_after);
		return (run_data->last_cmd_status);
	}
	run_data->cmd = cmd->and->cmd_after;
	return (runner_cmd(run_data, FORK_NOT));
}
