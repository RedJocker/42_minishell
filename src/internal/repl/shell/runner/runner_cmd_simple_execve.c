/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_simple_execve.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 23:53:03 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/18 19:55:03 by maurodri         ###   ########.fr       */
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
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/repl/shell/runner/runner_internal.h"

static const char	*g_not_found = "minishell: %s: command not found";
static const char	*g_is_dir = "minishell: %s: Is a directory";
static const char	*g_strerr = "minishell: %s: %s";
static const char	*g_file_not_found = \
	"minishell: %s: No such file or directory";

static void	runner_cmd_simple_execve_error_eacces(t_runner_data *run_data, \
				int err_num)
{
	char			*msg;
	struct stat		path_stat;
	const t_command	cmd = run_data->cmd;

	if (cmd->simple->cmd_argv[0][0] == '\0')
	{
		ft_asprintf(&msg, g_not_found, cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_FOUND, true);
	}
	stat(cmd->simple->cmd_argv[0], &path_stat);
	if (S_ISDIR(path_stat.st_mode) || (S_ISREG(path_stat.st_mode)
			&& !ft_strchr(cmd->simple->cmd_path, '/')))
	{
		if (ft_strchr(cmd->simple->cmd_path, '/'))
		{
			ft_asprintf(&msg, g_is_dir, cmd->simple->cmd_argv[0]);
			runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_EXEC, true);
			return ;
		}
		ft_asprintf(&msg, g_not_found, cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_FOUND, true);
		return ;
	}
	ft_asprintf(&msg, g_strerr, cmd->simple->cmd_argv[0], strerror(err_num));
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
		ft_asprintf(&msg, g_file_not_found, cmd->simple->cmd_argv[0]);
		runner_cmd_simple_panic(run_data, msg, EXIT_CMD_NOT_FOUND, true);
	}
	else
	{
		if (path)
			free(path);
		ft_asprintf(&msg, g_not_found, cmd->simple->cmd_argv[0]);
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
	ft_asprintf(&msg, g_strerr, cmd->simple->cmd_argv[0], strerror(err_num));
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
