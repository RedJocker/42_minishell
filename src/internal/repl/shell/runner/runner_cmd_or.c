/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_or.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 03:22:37 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 23:00:14 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_assert.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/repl/shell/runner/runner_internal.h"

sig_atomic_t	runner_cmd_or(t_runner_data *run_data)
{
	sig_atomic_t	status;
	const t_command	cmd = run_data->cmd;

	ft_assert(cmd->type == CMD_OR, "expected only cmd_or");
	run_data->backup_pids = ft_arraylist_add(run_data->backup_pids, \
								run_data->pids);
	run_data->pids = ft_arraylist_new((t_consumer) free);
	run_data->cmd = cmd->and->cmd_before;
	status = runner_cmd(run_data, FORK_NOT);
	status = status << 8;
	runner_wait(run_data, &status);
	ft_arraylist_destroy(run_data->pids);
	run_data->pids = ft_arraylist_pop(run_data->backup_pids);
	if (WIFEXITED(status))
		run_data->last_cmd_status = (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		run_data->last_cmd_status = runner_exit_signal(status);
	if (run_data->last_cmd_status == EXIT_OK \
		|| run_data->last_cmd_status == EXIT_SIGINT)
	{
		command_close_ios(cmd->or->cmd_after);
		return (run_data->last_cmd_status);
	}
	run_data->cmd = cmd->and->cmd_after;
	return (runner_cmd(run_data, FORK_NOT));
}
