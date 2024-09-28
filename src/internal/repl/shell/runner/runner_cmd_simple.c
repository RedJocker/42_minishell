/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_simple.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 21:36:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/28 06:04:38 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_assert.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal/default.h"
// #include "internal/env/envp.h"
// #include "internal/env/env.h"
#include "internal/repl/shell/command/command_internal/command_internal.h"
#include "internal/repl/shell/command/io_handler.h"
#include "runner.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "builtins/builtins.h"

void	runner_cmd_simple_panic(
	t_command cmd, char *msg, sig_atomic_t status_code)
{
	ft_puterrl(msg);
	free(msg);
	command_destroy(cmd);
	exit(status_code);
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

sig_atomic_t	runner_cmd_builtin(
	t_builtin builtin, t_command cmd, t_arraylist *pids)
{
	(void) pids;
	if (builtin == BUILTIN_ECHO)
		runner_cmd_builtin_echo(cmd);
	// TODO: check need for return status_code 
	exit(0);
	return (0);
}

static void	runner_cmd_simple_execve_error_eacces(t_command cmd, int err_num)
{
	char		*msg;
	struct stat	path_stat;

	if (cmd->simple->cmd_argv[0][0] == '\0')
	{
		ft_asprintf(
			&msg, "bash: %s: command not found", cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_FOUND);
	}
	stat(cmd->simple->cmd_argv[0], &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		if (ft_strchr(cmd->simple->cmd_path, '/'))
		{
			ft_asprintf(
				&msg, "bash: %s: Is a directory", cmd->simple->cmd_argv[0]);
			runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_EXECUTABLE);
		}
		else
		{
			ft_asprintf(
				&msg, "bash: %s: command not found", cmd->simple->cmd_argv[0]);
			runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_FOUND);
		}
	}
	else
	{
		ft_asprintf(
			&msg, "bash: %s: %s",
			cmd->simple->cmd_argv[0], strerror(err_num));
		runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_EXECUTABLE);
	}
}

static void	runner_cmd_simple_execve_error_enoent(t_command cmd)
{
	char	*msg;

	if (ft_strchr(cmd->simple->cmd_path, '/'))
	{
		ft_asprintf(&msg, "bash: %s: No such file or directory",
			cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_FOUND);
	}
	else
	{
		ft_asprintf(&msg, "bash: %s: command not found",
			cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(cmd, msg, EXIT_COMMAND_NOT_FOUND);
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
	return (runner_cmd_simple_panic(cmd, msg, err_num));
}

sig_atomic_t	runner_cmd_simple_execve(t_command cmd)
{	
	execve(cmd->simple->cmd_path, cmd->simple->cmd_argv, cmd->simple->cmd_envp);
	runner_cmd_simple_execve_error(cmd, errno);
	ft_assert(0, "unexpected execution");
	return (-1);
}

sig_atomic_t	runner_cmd_simple(t_command cmd, t_arraylist *pids)
{
	pid_t			*pid;
	sig_atomic_t	status;
	char			*err_msg;
	t_builtin		maybe_builtin;

	ft_assert(cmd->type == CMD_SIMPLE, "expected only cmd_simple");
	pid = malloc(sizeof(pid_t));
	*pid = fork();
	status = EXIT_OK;
	if (*pid < 0)
		exit(EXIT_FORK_FAIL);
	if (*pid == 0)
	{
		free(pid);
		cmd->simple->cmd_envp = __environ; // TODO: change to get_envp
		cmd->simple->cmd_path = parse_bin_path(cmd->simple->cmd_argv[DEFAULT]); // TODO: Check if the name of the parse_bin_path make sense.
		// cmd->simple->cmd_path = (get_bin_path_with_envp(cmd->simple->cmd_argv[0],
		// 				get_envp()));
		ft_arraylist_destroy(*pids);
		if (!io_handlers_redirect(cmd->io_handlers, &err_msg))
			runner_cmd_simple_panic(cmd, ft_strdup(err_msg), 1);
		maybe_builtin = runner_maybe_cmd_builtin(cmd);
		if (maybe_builtin)
		{
			status = runner_cmd_builtin(maybe_builtin, cmd, pids);
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
