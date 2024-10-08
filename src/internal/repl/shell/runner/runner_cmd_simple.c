/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_simple.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 21:36:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/09 04:40:04 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "ft_assert.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/env/envp.h"
#include "internal/env/env.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/signal/signal.h"
#include "runner.h"
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "builtins/builtins.h"

void	runner_cmd_simple_panic(t_runner_data *run_data, char *msg, \
	sig_atomic_t status_code, bool should_exit)
{
	ft_puterrl(msg);
	free(msg);
	if (should_exit)
		runner_cmd_simple_exit_status(run_data, status_code);
}


void close_fdp(int *fdp)
{
	close(*fdp);
}


void	runner_cmd_simple_exit_status(
	t_runner_data *runner_data, sig_atomic_t status)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	env_destroy();
	command_destroy(runner_data->base_cmd);
	ft_arraylist_foreach(*runner_data->pipes_to_close, (t_consumer) close_fdp);
	ft_arraylist_destroy(*runner_data->pipes_to_close);
	ft_arraylist_destroy(*runner_data->pids);
	exit(status);
}

t_builtin	runner_maybe_cmd_builtin(t_command cmd)
{
	char	*invocation;

	if (cmd->type != CMD_SIMPLE || cmd->simple->cmd_argc <= 0)
		return (0);
	invocation = cmd->simple->cmd_argv[0];
	if (ft_strncmp(invocation, "echo", 5) == 0)
		return (BUILTIN_ECHO);
	return (NOT_BUILTIN);
}

sig_atomic_t	runner_cmd_builtin(t_builtin builtin, t_command cmd, bool shoul_exit)
{
	sig_atomic_t	status;

	(void) shoul_exit; //TODO: check if should_exit is needed
	status = EXIT_OK;
	if (builtin == BUILTIN_ECHO)
		status = runner_cmd_builtin_echo(cmd);
	return (status);
}

sig_atomic_t	runner_cmd_builtin_nofork(t_builtin builtin, \
	t_runner_data *run_data)
{
	int				copy_fds[2];
	sig_atomic_t	status;
	char			*err_msg;
	const t_command	cmd = run_data->cmd;

	copy_fds[0] = dup(STDIN_FILENO);
	copy_fds[1] = dup(STDOUT_FILENO);
	cmd->simple->cmd_envp = get_envp();
	if (!io_handlers_redirect(cmd->io_handlers, &err_msg))
	{
		close(copy_fds[0]);
		close(copy_fds[1]);
		runner_cmd_simple_panic(
			run_data, ft_strdup(err_msg), EXIT_REDIRECT_FAIL, false);
		return (EXIT_REDIRECT_FAIL);
	}
	status = runner_cmd_builtin(builtin, cmd, false);
	dup2(copy_fds[0], STDIN_FILENO);
	dup2(copy_fds[1], STDOUT_FILENO);
	close(copy_fds[0]);
	close(copy_fds[1]);
	ft_arraylist_foreach(*run_data->pipes_to_close, (t_consumer) close_fdp);
	return (status);
}


static void	runner_cmd_simple_execve_error_eacces(
	t_runner_data *run_data, int err_num)
{
	char		*msg;
	struct stat	path_stat;
	const t_command	cmd = run_data->cmd;

	if (cmd->simple->cmd_argv[0][0] == '\0')
	{
		ft_asprintf(
			&msg, "bash: %s: command not found", cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(run_data, msg, EXIT_COMMAND_NOT_FOUND, true);
	}
	stat(cmd->simple->cmd_argv[0], &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		if (ft_strchr(cmd->simple->cmd_path, '/'))
		{
			ft_asprintf(
				&msg, "bash: %s: Is a directory", cmd->simple->cmd_argv[0]);
			runner_cmd_simple_panic(run_data, msg, EXIT_COMMAND_NOT_EXECUTABLE, true);
		}
		else
		{
			ft_asprintf(
				&msg, "bash: %s: command not found", cmd->simple->cmd_argv[0]);
			runner_cmd_simple_panic(run_data, msg, EXIT_COMMAND_NOT_FOUND, true);
		}
	}
	else
	{
		ft_asprintf(
			&msg, "bash: %s: %s",
			cmd->simple->cmd_argv[0], strerror(err_num));
		runner_cmd_simple_panic(run_data, msg, EXIT_COMMAND_NOT_EXECUTABLE, true);
	}
}

static void	runner_cmd_simple_execve_error_enoent(t_runner_data *run_data)
{
	char	*msg;
	const t_command	cmd = run_data->cmd;

	if (ft_strchr(cmd->simple->cmd_path, '/'))
	{
		ft_asprintf(&msg, "bash: %s: No such file or directory",
			cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(run_data, msg, EXIT_COMMAND_NOT_FOUND, true);
	}
	else
	{
		ft_asprintf(&msg, "bash: %s: command not found",
			cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(run_data, msg, EXIT_COMMAND_NOT_FOUND, true);
	}
}

static void	runner_cmd_simple_execve_error(t_runner_data *run_data, int err_num)
{
	char	*msg;
	const t_command	cmd = run_data->cmd;

	if (err_num == ENOENT)
		return (runner_cmd_simple_execve_error_enoent(run_data));
	else if (err_num == EACCES)
		return (runner_cmd_simple_execve_error_eacces(run_data, err_num));
	ft_asprintf(&msg, "bash: %s: %s",
		cmd->simple->cmd_argv[0], strerror(err_num));
	return (runner_cmd_simple_panic(run_data, msg, err_num, true));
}

sig_atomic_t	runner_cmd_simple_execve(t_runner_data *run_data)
{
	const t_command	cmd = run_data->cmd;

	execve(cmd->simple->cmd_path, cmd->simple->cmd_argv, cmd->simple->cmd_envp);
	runner_cmd_simple_execve_error(run_data, errno);
	ft_assert(0, "unexpected execution");
	return (-1);
}

sig_atomic_t	runner_cmd_simple(
	t_runner_data *run_data, t_fork_flag should_fork)
{
	pid_t			*pid;
	sig_atomic_t	status;
	char			*err_msg;
	t_builtin		maybe_builtin;
	const t_command	cmd = run_data->cmd;

	ft_assert(cmd->type == CMD_SIMPLE, "expected only cmd_simple");
	maybe_builtin = runner_maybe_cmd_builtin(cmd);
	if (!should_fork && maybe_builtin)
		return (runner_cmd_builtin_nofork(maybe_builtin, run_data));
	pid = malloc(sizeof(pid_t));
	*pid = fork();
	status = EXIT_OK;
	if (*pid < 0)
		exit(EXIT_FORK_FAIL);
	if (*pid == 0)
	{
		free(pid);
		if (cmd->simple->cmd_argc == DEFAULT)
			runner_cmd_simple_exit_status(run_data, EXIT_OK);
		cmd->simple->cmd_envp = get_envp();
		cmd->simple->cmd_path = env_get_bin(cmd->simple->cmd_argv[DEFAULT]);
		if (!io_handlers_redirect(cmd->io_handlers, &err_msg))
			runner_cmd_simple_panic(run_data, \
				ft_strdup(err_msg), EXIT_REDIRECT_FAIL, true);
		signals_afterfork();
		if (maybe_builtin)
		{
			//ft_printf("fork builtin %s %d\n", cmd->debug_id, cmd->type);
			status = runner_cmd_builtin(maybe_builtin, cmd, true);
			runner_cmd_simple_exit_status(run_data, status);
			ft_assert(0, "unexpected line executed");
		}
		return (runner_cmd_simple_execve(run_data));
	}
	else
	{
		//TODO: change status to function error handling
		*run_data->pids = ft_arraylist_add(*run_data->pids, pid);
		io_handlers_close(cmd->io_handlers);
		if (!(*run_data->pids))
			status = EXIT_OUT_OF_MEMORY;
	}
	return (status);
}
