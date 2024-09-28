/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Updated: 2024/09/28 01:06:13 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "ft_assert.h"
#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/repl/shell/runner/expand/expand.h"
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
	runner_cmd(cmd->pipe->cmd_before, pids, last_status_code);
	runner_cmd(cmd->pipe->cmd_after, pids, last_status_code);
	return (status);
}

sig_atomic_t	runner_cmd(
	t_command cmd, t_arraylist *pids, sig_atomic_t last_cmd_status)
{
	sig_atomic_t	status;

	status = DEFAULT;
	expand(cmd, last_cmd_status);
	//ft_strarr_printfd(cmd->simple->cmd_argv, 1);
	if (cmd->type == CMD_SIMPLE)
		status = runner_cmd_simple(cmd, pids);
	else if (cmd->type == CMD_INVALID)
		status = runner_cmd_invalid(cmd, pids);
	else if (cmd->type == CMD_PIPE)
		status = runner_cmd_pipe(cmd, pids, last_cmd_status);
	return (status);
}

sig_atomic_t	runner(t_command cmd, sig_atomic_t last_cmd_status)
{
	t_arraylist		pids;
	int				pids_len;
	sig_atomic_t	status;
	int				i;

	if (cmd->type == CMD_EOF)
	{
		command_destroy(cmd);
		ft_putendl("exit");
		env_destroy();
		exit(last_cmd_status);
	}
	pids = ft_arraylist_new(free);
	status = runner_cmd(cmd, &pids, last_cmd_status);
	pids_len = ft_arraylist_len(pids);
	i = -1;
	while (++i < pids_len - 1)
		waitpid(*((pid_t *) ft_arraylist_get(pids, i)), 0, 0);
	if (pids_len > 0)
		waitpid(*((pid_t *) ft_arraylist_get(pids, i)), &status, 0);
	ft_arraylist_destroy(pids);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}
