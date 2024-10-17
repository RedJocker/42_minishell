/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:38:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/17 16:29:46 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
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


sig_atomic_t	runner_exit_signal(sig_atomic_t	status)
{
	sig_atomic_t	signal_num;

	signal_num = WTERMSIG(status);
	if (signal_num == SIGQUIT)
		ft_putstr("Quit (core dumped)\n");
	return (SIG_BASE + signal_num);
}


sig_atomic_t	runner_cmd_invalid(t_command cmd, t_runner_data *runner_data)
{
	(void) runner_data;
	ft_assert(cmd->type == CMD_INVALID, "expected only invalid");
	ft_puterrl(cmd->invalid->msg);
	return (cmd->invalid->status);
}

// pipe_fds[0] read, pipe_fds[1] write
sig_atomic_t	runner_cmd_pipe(t_runner_data *run_data)
{
	sig_atomic_t	status;
	int				pipe_fds[2];
	const t_command cmd = run_data->cmd;
	int				*to_close;

	ft_assert(cmd->type == CMD_PIPE, "expected only cmd_pipe");
	status = EXIT_OK;
	pipe(pipe_fds);
	command_add_pipe_io(cmd->pipe->cmd_before, pipe_fds[1], IO_OUT);
	command_add_pipe_io(cmd->pipe->cmd_after, pipe_fds[0], IO_IN);
	to_close = malloc(sizeof(int));
	*to_close = pipe_fds[0];
	run_data->pipes_to_close = ft_arraylist_add(
			run_data->pipes_to_close, to_close);
	run_data->cmd = cmd->pipe->cmd_before;
	runner_cmd(run_data, FORK_YES);
	run_data->cmd = cmd->pipe->cmd_after;
	runner_cmd(run_data, FORK_YES);
	return (status);
}

sig_atomic_t	runner_cmd_and(t_runner_data *run_data)
{
	sig_atomic_t	status;
	const t_command cmd = run_data->cmd;
	int				pids_len;
	int				i;

	ft_assert(cmd->type == CMD_AND, "expected only cmd_and");
	run_data->backup_pids = run_data->pids;
	run_data->pids = ft_arraylist_new((t_consumer) free);
	run_data->cmd = cmd->and->cmd_before;
	status = runner_cmd(run_data, FORK_NOT);
	status = status << 8;
	pids_len = ft_arraylist_len(run_data->pids);
	i = -1;
	while (++i < pids_len - 1)
		waitpid(*((pid_t *) ft_arraylist_get(run_data->pids, i)), 0, 0);
	if (pids_len > 0)
		waitpid(*((pid_t *) ft_arraylist_get(run_data->pids, i)), &status, 0);
	ft_arraylist_destroy(run_data->pids);
	run_data->pids = run_data->backup_pids;
	run_data->backup_pids = NULL;
	if (WIFSIGNALED(status))
			return (runner_exit_signal(status));
	if (WIFEXITED(status))
		run_data->last_cmd_status = (WEXITSTATUS(status));
	if (run_data->last_cmd_status != 0)
		return (run_data->last_cmd_status);
	run_data->cmd = cmd->and->cmd_after;
	return (runner_cmd(run_data, FORK_NOT));
}


sig_atomic_t	runner_cmd_or(t_runner_data *run_data)
{
	sig_atomic_t	status;
	const t_command cmd = run_data->cmd;
	int				pids_len;
	int				i;

	ft_assert(cmd->type == CMD_OR, "expected only cmd_or");
	run_data->backup_pids = run_data->pids;
	run_data->pids = ft_arraylist_new((t_consumer) free);
	run_data->cmd = cmd->and->cmd_before;
	status = runner_cmd(run_data, FORK_NOT);
	status = status << 8;
	pids_len = ft_arraylist_len(run_data->pids);
	i = -1;
	while (++i < pids_len - 1)
		waitpid(*((pid_t *) ft_arraylist_get(run_data->pids, i)), 0, 0);
	if (pids_len > 0)
		waitpid(*((pid_t *) ft_arraylist_get(run_data->pids, i)), &status, 0);
	ft_arraylist_destroy(run_data->pids);
	run_data->pids = run_data->backup_pids;
	run_data->backup_pids = NULL;
	if (WIFEXITED(status))
		run_data->last_cmd_status = (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		run_data->last_cmd_status = runner_exit_signal(status);
	if (run_data->last_cmd_status == 0)
		return (EXIT_OK);
	run_data->cmd = cmd->and->cmd_after;
	return (runner_cmd(run_data, FORK_NOT));
}


sig_atomic_t	runner_cmd(t_runner_data *run_data, t_fork_flag should_fork)
{
	sig_atomic_t		status;
	const t_command		cmd = run_data->cmd;

	status = DEFAULT;
	expand(cmd, run_data->last_cmd_status);
	runner_heredoc_to_fd(cmd);
	//ft_strarr_printfd(cmd->simple->cmd_argv, 1);
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
	run_data->last_cmd_status = status;
	return (status);
}

void runner_cmd_eof(t_runner_data *run_data)
{
	ft_putendl("exit");
	runner_cmd_simple_exit_status(run_data, run_data->last_cmd_status);
}

sig_atomic_t	runner(t_command cmd, sig_atomic_t last_cmd_status)
{
	t_runner_data	run_data;
	int				pids_len;
	sig_atomic_t	status;
	int				i;

	status = EXIT_OK;
	runner_data_init(&run_data, cmd, last_cmd_status);
	if (cmd->type == CMD_EOF)
		runner_cmd_eof(&run_data);
	runner_heredoc_prompt(run_data.base_cmd);
	status = runner_cmd(&run_data, FORK_NOT);
	status = status << 8;
	pids_len = ft_arraylist_len(run_data.pids);
	i = -1;
	while (++i < pids_len - 1)
		waitpid(*((pid_t *) ft_arraylist_get(run_data.pids, i)), 0, 0);
	if (pids_len > 0)
		waitpid(*((pid_t *) ft_arraylist_get(run_data.pids, i)), &status, 0);
	runner_data_clean(&run_data);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (runner_exit_signal(status));
	ft_assert(0, "unexpected line execution");
	return (0);
}
