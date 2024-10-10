/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 03:19:01 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/10 05:06:40 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_string.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/env/envp.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/repl/shell/runner/builtins/builtins.h"
#include "internal/repl/shell/runner/runner.h"
#include "internal/repl/shell/command/command.h"

static t_builtins	builtins_init(void);

t_builtin_id	runner_maybe_cmd_builtin(t_command cmd)
{
	int					builtins_len;
	const t_builtins	builtins = builtins_init();
	char				*invocation;

	builtins_len = DEFAULT_INIT;
	if (cmd->type != CMD_SIMPLE || cmd->simple->cmd_argc <= DEFAULT)
		return (NOT_BUILTIN);
	invocation = cmd->simple->cmd_argv[DEFAULT];
	while (builtins.builtin[++builtins_len].invocation)
	{
		if (ft_strncmp(invocation, \
			builtins.builtin[builtins_len].invocation, \
			ft_strlen(builtins.builtin[builtins_len].invocation) \
			+ NULL_BYTE) == DEFAULT)
			return (builtins.builtin[builtins_len].id);
	}
	return (NOT_BUILTIN);
}

sig_atomic_t	runner_cmd_builtin(t_builtin_id builtin, t_command cmd)
{
	int					builtins_len;
	sig_atomic_t		status;
	const t_builtins	builtins = builtins_init();

	builtins_len = DEFAULT_INIT;
	status = EXIT_OK;
	while (builtins.builtin[++builtins_len].invocation)
	{
		if (builtin == builtins.builtin[builtins_len].id)
		{
			status = builtins.builtin[builtins_len].fn(cmd);
			break ;
		}
	}
	return (status);
}

sig_atomic_t	runner_cmd_builtin_nofork(t_builtin_id builtin, \
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
	status = runner_cmd_builtin(builtin, cmd);
	dup2(copy_fds[0], STDIN_FILENO);
	dup2(copy_fds[1], STDOUT_FILENO);
	close(copy_fds[0]);
	close(copy_fds[1]);
	ft_arraylist_foreach(*run_data->pipes_to_close, \
		(t_consumer) close_fd_pipes);
	return (status);
}

static t_builtins	builtins_init(void)
{
	return ((t_builtins){.builtin = {
			{BUILTIN_ECHO, "echo", &runner_cmd_builtin_echo},
			{BUILTIN_EXPORT, "export", &runner_cmd_builtin_export},
			{NOT_BUILTIN, NULL, NULL},
		}});
}
