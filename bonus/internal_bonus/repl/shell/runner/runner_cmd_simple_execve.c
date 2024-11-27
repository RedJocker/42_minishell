/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_simple_execve.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 23:53:03 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/26 23:07:37 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ft_assert.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/env/env.h"
#include "internal_bonus/repl/shell/runner/runner_internal.h"

static char	*build_err_msg(
	t_runner_msg_error type,
	char *cmd,
	int err_num)
{
	char	*msg;

	msg = NULL;
	if (type == NOT_FOUND)
		ft_asprintf(&msg, "minishell: %s: command not found", cmd);
	else if (type == IS_DIR)
		ft_asprintf(&msg, "minishell: %s: Is a directory", cmd);
	else if (type == STRERR)
		ft_asprintf(&msg, "minishell: %s: %s", cmd, strerror(err_num));
	else if (type == FILE_NOT_FOUND)
		ft_asprintf(&msg, \
			"minishell: %s: No such file or directory", cmd);
	return (msg);
}

static void	runner_cmd_simple_execve_error_eacces(t_runner_data *run_data, \
				int err_num)
{
	char			*msg;
	struct stat		path_stat;
	const t_command	cmd = run_data->cmd;

	if (cmd->simple->cmd_argv[0][0] == '\0')
	{
		msg = build_err_msg(NOT_FOUND, cmd->simple->cmd_argv[0], 0);
		runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_FOUND, true);
	}
	stat(cmd->simple->cmd_argv[0], &path_stat);
	if (S_ISDIR(path_stat.st_mode) || (S_ISREG(path_stat.st_mode)
			&& !ft_strchr(cmd->simple->cmd_path, '/')))
	{
		if (ft_strchr(cmd->simple->cmd_path, '/'))
		{
			msg = build_err_msg(IS_DIR, cmd->simple->cmd_argv[0], 0);
			runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_EXEC, true);
			return ;
		}
		msg = build_err_msg(NOT_FOUND, cmd->simple->cmd_argv[0], 0);
		runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_FOUND, true);
		return ;
	}
	msg = build_err_msg(STRERR, cmd->simple->cmd_argv[0], err_num);
	runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_EXEC, true);
}

static void	runner_cmd_simple_execve_error_enoent(t_runner_data *run_data)
{
	char			*msg;
	const t_command	cmd = run_data->cmd;
	char			*path;

	path = env_get_value("PATH");
	if (ft_strchr(cmd->simple->cmd_path, '/') || !path)
	{
		if (path)
			free(path);
		msg = build_err_msg(FILE_NOT_FOUND, cmd->simple->cmd_argv[0], 0);
		runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_FOUND, true);
	}
	else
	{
		if (path)
			free(path);
		msg = build_err_msg(NOT_FOUND, cmd->simple->cmd_argv[0], 0);
		runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_FOUND, true);
	}
}

static void	runner_cmd_simple_execve_error(t_runner_data *run_data, int err_num)
{
	char			*msg;
	const t_command	cmd = run_data->cmd;

	if (err_num == ENOENT)
		return (runner_cmd_simple_execve_error_enoent(run_data));
	else if (err_num == EACCES)
		return (runner_cmd_simple_execve_error_eacces(run_data, err_num));
	msg = build_err_msg(STRERR, cmd->simple->cmd_argv[0], err_num);
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
