/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_simple.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 21:36:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/29 16:51:20 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <readline/readline.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include "collection/ft_arraylist.h"
#include "ft_assert.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/env/envp.h"
#include "internal_bonus/env/env.h"
#include "internal_bonus/repl/shell/command/io_handler.h"
#include "internal_bonus/repl/shell/runner/builtins/builtins.h"
#include "internal_bonus/signal/signal.h"

void	runner_cmd_simple_panic(t_runner_data *run_data, char *msg, \
	sig_atomic_t status_code, bool should_exit)
{
	ft_puterrl(msg);
	free(msg);
	if (should_exit)
		runner_cmd_simple_exit_status(run_data, status_code);
}

void	close_fd_lst(t_arraylist fd_lst)
{
	int	*fd;
	int	i;
	int	len;

	i = -1;
	len = ft_arraylist_len(fd_lst);
	while (++i < len)
	{
		fd = ft_arraylist_get(fd_lst, i);
		close(*fd);
	}
}

void	runner_cmd_simple_exit_status(
	t_runner_data *runner_data, sig_atomic_t status)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	env_destroy();
	close_fd_lst(runner_data->pipes_to_close);
	runner_data_clean(runner_data);
	exit(status);
}

sig_atomic_t	runner_cmd_simple_child(t_runner_data *run_data, \
					t_builtin_id builtin, pid_t *pid, sig_atomic_t *status)
{
	const t_command		cmd = run_data->cmd;

	rl_clear_history();
	io_handlers_close(cmd->simple->pipes_to_close);
	run_data->is_main = 0;
	free(pid);
	if (!io_handlers_redirect(cmd->io_handlers))
		runner_cmd_simple_panic(run_data, \
			ft_strdup(io_handlers_get_error(cmd->io_handlers)), \
				EXIT_REDIRECT_FAIL, true);
	if (cmd->simple->cmd_argc == DEFAULT)
		runner_cmd_simple_exit_status(run_data, EXIT_OK);
	cmd->simple->cmd_envp = get_envp(ENVP_DEFAULT);
	cmd->simple->cmd_path = env_get_bin(cmd->simple->cmd_argv[DEFAULT]);
	signals_afterfork();
	if (builtin)
	{
		*status = runner_cmd_builtin(builtin, cmd);
		print_builtin_exit(builtin, run_data, *status);
		if (*status < 0 && builtin == BUILTIN_EXIT)
			*status *= -1;
		runner_cmd_simple_exit_status(run_data, *status);
		ft_assert(0, "unexpected line executed");
	}
	return (runner_cmd_simple_execve(run_data));
}

sig_atomic_t	runner_cmd_simple(t_runner_data *run_data, \
					t_fork_flag should_fork)
{
	pid_t			*pid;
	sig_atomic_t	status;
	t_builtin_id	builtin;
	const t_command	cmd = run_data->cmd;

	ft_assert(cmd->type == CMD_SIMPLE, "expected only cmd_simple");
	builtin = check_builtin(cmd);
	if (!should_fork && builtin)
		return (runner_cmd_builtin_without_fork(builtin, run_data));
	pid = malloc(sizeof(pid_t));
	*pid = fork();
	status = EXIT_OK;
	if (*pid < 0)
		exit(EXIT_FORK_FAIL);
	if (*pid == 0)
		return (runner_cmd_simple_child(run_data, builtin, pid, &status));
	else
	{
		run_data->pids = ft_arraylist_add(run_data->pids, pid);
		command_close_ios(cmd);
		if (!(run_data->pids))
			status = EXIT_OUT_OF_MEMORY;
	}
	return (status);
}
