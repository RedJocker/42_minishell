/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:38:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 04:06:05 by maurodri         ###   ########.fr       */
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
#include "internal/signal/signal.h"
#include "runner.h"
#include "runner_internal.h"
#include "signal.h"

sig_atomic_t	runner_exit_signal(sig_atomic_t	status)
{
	sig_atomic_t	signal_num;

	signal_num = WTERMSIG(status);
	if (signal_num == SIGQUIT)
		ft_putstr("Quit (core dumped)\n");
	return (SIG_BASE + signal_num);
}

sig_atomic_t	runner_normalize_status(sig_atomic_t status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (runner_exit_signal(status));
	ft_assert(0, "unexpected line execution");
	return (-1);
}

void	runner_wait(t_runner_data *run_data, sig_atomic_t *status)
{
	int				pids_len;
	int				i;

	pids_len = ft_arraylist_len(run_data->pids);
	i = -1;
	while (++i < pids_len - 1)
		waitpid(*((pid_t *) ft_arraylist_get(run_data->pids, i)), 0, 0);
	if (pids_len > 0)
		waitpid(*((pid_t *) ft_arraylist_get(run_data->pids, i)), status, 0);
}

sig_atomic_t	runner(t_command cmd, sig_atomic_t last_cmd_status)
{
	t_runner_data	run_data;
	sig_atomic_t	status;

	status = EXIT_OK;
	runner_data_init(&run_data, cmd, last_cmd_status);
	if (cmd->type == CMD_EOF)
		runner_cmd_eof(&run_data);
	signals_heredoc();
	runner_heredoc_prompt(run_data.base_cmd);
	signals_initializer(false);
	if (!(signal_status(DEFAULT, GET) == SIGINT))
		status = runner_cmd(&run_data, FORK_NOT);
	else
		status = EXIT_SIGINT;
	status = status << 8;
	runner_wait(&run_data, &status);
	runner_data_clean(&run_data);
	return (runner_normalize_status(status));
}
