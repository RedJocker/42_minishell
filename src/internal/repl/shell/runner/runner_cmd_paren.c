/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_paren.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 03:14:47 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 04:19:09 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "collection/ft_arraylist.h"
#include "ft_assert.h"
#include "ft_stdio.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/repl/shell/runner/expand/expand.h"
#include "runner.h"
#include "internal/signal/signal.h"
#include "signal.h"
#include "runner_internal.h"

static void	runner_cmd_paren_init(
	t_runner_data *run_data, t_command cmd, pid_t **pid, sig_atomic_t *stat)
{
	run_data->backup_pids = ft_arraylist_add(\
								run_data->backup_pids, run_data->pids);
	run_data->pids = ft_arraylist_new((t_consumer) free);
	run_data->cmd = cmd->paren->cmd;
	*pid = malloc(sizeof(pid_t));
	*stat = EXIT_OK;
}

sig_atomic_t	runner_cmd_paren(t_runner_data *run_data)
{
	const t_command	cmd = run_data->cmd;
	pid_t			*pid;
	sig_atomic_t	stat;

	ft_assert(cmd->type == CMD_PAREN, "expected only cmd_paren");
	runner_cmd_paren_init(run_data, cmd, &pid, &stat);
	*pid = fork();
	if (*pid < 0)
		exit(EXIT_FORK_FAIL);
	if (*pid == 0)
	{
		free(pid);
		pid = NULL;
		stat = runner_cmd(run_data, FORK_NOT);
		stat = stat << 8;
		runner_wait(run_data, &stat);
		stat = runner_normalize_status(stat);
		runner_cmd_simple_exit_status(run_data, stat);
	}
	command_close_ios(cmd);
	ft_arraylist_destroy(run_data->pids);
	run_data->pids = ft_arraylist_pop(run_data->backup_pids);
	run_data->pids = ft_arraylist_add(run_data->pids, pid);
	return (stat);
}
