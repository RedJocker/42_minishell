/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:38:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/07 16:55:48 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "collection/ft_arraylist.h"
#include "ft_assert.h"
#include "ft_stdio.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/repl/shell/runner/expand/expand.h"
#include "internal/signal/signal.h"
#include "runner.h"

void	command_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir);

sig_atomic_t	runner_cmd_invalid(t_command cmd, t_arraylist *pids)
{
	(void) pids;
	ft_assert(cmd->type == CMD_INVALID, "expected only invalid");
	ft_puterrl(cmd->invalid->msg);
	return (cmd->invalid->status);
}

// pipe_fds[0] read, pipe_fds[1] write
sig_atomic_t	runner_cmd_pipe(
	t_command cmd, t_arraylist *pids, sig_atomic_t last_status_code)
{
	sig_atomic_t	status;
	int				pipe_fds[2];

	ft_assert(cmd->type == CMD_PIPE, "expected only cmd_pipe");
	status = EXIT_OK;
	pipe(pipe_fds);
	command_add_pipe_io(cmd->pipe->cmd_before, pipe_fds[1], IO_OUT);
	command_add_pipe_io(cmd->pipe->cmd_after, pipe_fds[0], IO_IN);
	runner_cmd(cmd->pipe->cmd_before, pids, last_status_code, true);
	runner_cmd(cmd->pipe->cmd_after, pids, last_status_code, true);
	return (status);
}

sig_atomic_t	runner_cmd(
	t_command cmd, t_arraylist *pids, sig_atomic_t last_cmd_status, bool should_fork)
{
	sig_atomic_t	status;

	status = DEFAULT;
	expand(cmd, last_cmd_status);
	//ft_strarr_printfd(cmd->simple->cmd_argv, 1);
	if (cmd->type == CMD_SIMPLE)
		status = runner_cmd_simple(cmd, pids, should_fork);
	else if (cmd->type == CMD_INVALID)
		status = runner_cmd_invalid(cmd, pids);
	else if (cmd->type == CMD_PIPE)
		status = runner_cmd_pipe(cmd, pids, last_cmd_status);
	return (status);
}

sig_atomic_t	runner_exit_signal(sig_atomic_t	status)
{
	sig_atomic_t	signal_num;

	signal_num = WTERMSIG(status);
	if (signal_num == SIGQUIT)
		ft_putstr("Quit (core dumped)\n");
	return (SIG_BASE + signal_num);
}

void runner_cmd_eof(t_command cmd, sig_atomic_t last_cmd_status)
{
	command_destroy(cmd);
	ft_putendl("exit");
	env_destroy();
	exit(last_cmd_status);
}

void io_handler_heredoc(t_io_handler *io)
{
	ft_printf("io_handler_heredoc %d\n", io->type);
	if (io->type != IO_HEREDOC)
		return ;
	ft_printf("heredoc %s\n", io->heredoc_limiter);
}


void io_handlers_heredoc(t_arraylist ios)
{
	ft_printf("io_handlers_heredoc\n");
	ft_arraylist_foreach(ios, (t_consumer) io_handler_heredoc);
}

void runner_heredoc(t_command cmd)
{
	ft_printf("runner heredoc %s\n", cmd->debug_id);
	if (cmd->type == CMD_SIMPLE)
		io_handlers_heredoc(cmd->io_handlers);
	else if (cmd->type == CMD_PIPE)
	{
		runner_heredoc(cmd->pipe->cmd_before);
		runner_heredoc(cmd->pipe->cmd_after);
	}
	else if (cmd->type == CMD_INVALID)
		;
	else
		ft_assert(0, "runner_heerdoc unexpected cmd type");
}

sig_atomic_t	runner(t_command cmd, sig_atomic_t last_cmd_status)
{
	t_arraylist		pids;
	int				pids_len;
	sig_atomic_t	status;
	int				i;

	status = EXIT_OK;
	if (cmd->type == CMD_EOF)
		runner_cmd_eof(cmd, last_cmd_status);
	runner_heredoc(cmd);
	pids = ft_arraylist_new(free);
	status = runner_cmd(cmd, &pids, last_cmd_status, 0);
	command_destroy(cmd);
	status = status << 8;
	pids_len = ft_arraylist_len(pids);
	i = -1;
	while (++i < pids_len - 1)
		waitpid(*((pid_t *) ft_arraylist_get(pids, i)), 0, 0);
	if (pids_len > 0)
		waitpid(*((pid_t *) ft_arraylist_get(pids, i)), &status, 0);
	ft_arraylist_destroy(pids);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (runner_exit_signal(status));
	ft_assert(0, "unexpected line execution");
	return (0);
}
