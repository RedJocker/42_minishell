/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_simple.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 21:36:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/02 23:12:08 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_assert.h"
#include "ft_stdio.h"
#include "ft_string.h"
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

void	runner_cmd_simple_panic(
	t_command cmd, char *msg, sig_atomic_t status_code, bool should_exit)
{
	ft_puterrl(msg);
	free(msg);
	if (should_exit)
	{
		command_destroy(cmd);
		exit(status_code);
	}
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
	char 			*msg;

	msg = NULL;
	status = EXIT_OK;
	if (builtin == BUILTIN_ECHO)
		status = runner_cmd_builtin_echo(cmd);
	if (status != EXIT_OK)
		runner_cmd_simple_panic(cmd, msg, status, shoul_exit);
	return (status);
}

sig_atomic_t	runner_cmd_builtin_nofork(t_builtin builtin, t_command cmd)
{
	int 			copy_fds[2];
	sig_atomic_t	status;
	char 			*err_msg;

	
	copy_fds[0] = dup(STDIN_FILENO);
	copy_fds[1] = dup(STDOUT_FILENO);
	if (!io_handlers_redirect(cmd->io_handlers, &err_msg))
	{
		runner_cmd_simple_panic(cmd, ft_strdup(err_msg), EXIT_REDIRECT_FAIL, false);
		return (EXIT_REDIRECT_FAIL);
	}
	status = runner_cmd_builtin(builtin, cmd, false);
	dup2(copy_fds[0], STDIN_FILENO);
	dup2(copy_fds[1], STDOUT_FILENO);
	return (status);
}


static void	runner_cmd_simple_execve_error_eacces(t_command cmd, int err_num)
{
	char		*msg;
	struct stat	path_stat;

	if (cmd->simple->cmd_argv[0][0] == '\0')
	{
		ft_asprintf(
			&msg, "bash: %s: command not found", cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_FOUND, true);
	}
	stat(cmd->simple->cmd_argv[0], &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		if (ft_strchr(cmd->simple->cmd_path, '/'))
		{
			ft_asprintf(
				&msg, "bash: %s: Is a directory", cmd->simple->cmd_argv[0]);
			runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_EXECUTABLE, true);
		}
		else
		{
			ft_asprintf(
				&msg, "bash: %s: command not found", cmd->simple->cmd_argv[0]);
			runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_FOUND, true);
		}
	}
	else
	{
		ft_asprintf(
			&msg, "bash: %s: %s",
			cmd->simple->cmd_argv[0], strerror(err_num));
		runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_EXECUTABLE, true);
	}
}

static void	runner_cmd_simple_execve_error_enoent(t_command cmd)
{
	char	*msg;

	if (ft_strchr(cmd->simple->cmd_path, '/'))
	{
		ft_asprintf(&msg, "bash: %s: No such file or directory",
			cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_FOUND, true);
	}
	else
	{
		ft_asprintf(&msg, "bash: %s: command not found",
			cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_FOUND, true);
	}
}

static void	runner_cmd_simple_execve_error(t_command cmd, int err_num)
{
	char	*msg;

	if (err_num == ENOENT)
		return (runner_cmd_simple_execve_error_enoent(cmd));
	else if (err_num == EACCES)
		return (runner_cmd_simple_execve_error_eacces(cmd, err_num));
	ft_asprintf(&msg, "bash: %s: %s",
		cmd->simple->cmd_argv[0], strerror(err_num));
	return (runner_cmd_simple_panic(cmd, msg, err_num, true));
}

sig_atomic_t	runner_cmd_simple_execve(t_command cmd)
{	
	execve(cmd->simple->cmd_path, cmd->simple->cmd_argv, cmd->simple->cmd_envp);
	runner_cmd_simple_execve_error(cmd, errno);
	ft_assert(0, "unexpected execution");
	return (-1);
}

sig_atomic_t	runner_cmd_simple(t_command cmd, t_arraylist *pids, bool should_fork)
{
	pid_t			*pid;
	sig_atomic_t	status;
	char			*err_msg;
	t_builtin		maybe_builtin;

	ft_assert(cmd->type == CMD_SIMPLE, "expected only cmd_simple");
	maybe_builtin = runner_maybe_cmd_builtin(cmd);
	if (!should_fork && maybe_builtin)
		return (runner_cmd_builtin_nofork(maybe_builtin, cmd));
	pid = malloc(sizeof(pid_t));
	*pid = fork();
	status = EXIT_OK;
	if (*pid < 0)
		exit(EXIT_FORK_FAIL);
	if (*pid == 0)
	{
		free(pid);
		cmd->simple->cmd_envp = get_envp();
		cmd->simple->cmd_path = env_get_bin(cmd->simple->cmd_argv[DEFAULT]);
		ft_arraylist_destroy(*pids);
		if (!io_handlers_redirect(cmd->io_handlers, &err_msg))
			runner_cmd_simple_panic(cmd, ft_strdup(err_msg), EXIT_REDIRECT_FAIL, true);
		signals_afterfork();
		if (maybe_builtin)
		{
			status = runner_cmd_builtin(maybe_builtin, cmd, true);
			return (status);
		}
		return (runner_cmd_simple_execve(cmd));
	}
	else
	{
		//TODO: change status to function error handling
		*pids = ft_arraylist_add(*pids, pid);
		io_handlers_close(cmd->io_handlers);
		if (!(*pids))
			status = EXIT_OUT_OF_MEMORY;
	}
	return (status);
}
