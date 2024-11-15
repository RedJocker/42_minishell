/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 04:03:46 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 04:08:45 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/repl/shell/runner/expand/expand.h"
#include "runner_internal.h"
#include "ft_assert.h"
#include "ft_util.h"
#include <sys/wait.h>

sig_atomic_t	runner_cmd_invalid(t_command cmd, t_runner_data *runner_data)
{
	runner_data->last_cmd_status = cmd->invalid->status;
	ft_assert(cmd->type == CMD_INVALID, "expected only invalid");
	ft_puterrl(cmd->invalid->msg);
	return (cmd->invalid->status);
}

void	runner_cmd_eof(t_runner_data *run_data)
{
	ft_putendl("exit");
	runner_cmd_simple_exit_status(run_data, run_data->last_cmd_status);
}

sig_atomic_t	runner_cmd(t_runner_data *run_data, t_fork_flag should_fork)
{
	sig_atomic_t		status;
	const t_command		cmd = run_data->cmd;

	status = DEFAULT;
	expand(cmd, run_data->last_cmd_status);
	runner_heredoc_to_fd(cmd);
	if (cmd->type == CMD_SIMPLE)
		status = runner_cmd_simple(run_data, should_fork);
	else if (cmd->type == CMD_INVALID)
		status = runner_cmd_invalid(cmd, run_data);
	else if (cmd->type == CMD_PIPE)
		status = runner_cmd_pipe(run_data);
	else if (cmd->type == CMD_AND)
		status = runner_cmd_and(run_data);
	else if (cmd->type == CMD_OR)
		status = runner_cmd_or(run_data);
	else if (cmd->type == CMD_PAREN)
		status = runner_cmd_paren(run_data);
	run_data->last_cmd_status = status;
	return (status);
}
