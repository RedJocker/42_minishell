/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_cmd_paren.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 03:14:47 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/29 16:57:19 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "collection/ft_arraylist.h"
#include "ft_assert.h"
#include "ft_util.h"
#include "internal_bonus/default.h"
#include "internal_bonus/repl/shell/command/command.h"
#include "internal_bonus/repl/shell/runner/runner_internal.h"

static void	runner_cmd_paren_child(
				t_runner_data *run_data,
				sig_atomic_t *stat,
				pid_t *pid);

static void	runner_cmd_paren_init(t_runner_data *run_data, t_command cmd, \
				pid_t **pid, sig_atomic_t *stat)
{
	run_data->backup_pids = ft_arraylist_add(run_data->backup_pids, \
								run_data->pids);
	run_data->pids = ft_arraylist_new((t_consumer) free);
	run_data->cmd = cmd->paren->cmd;
	*pid = malloc(sizeof(pid_t));
	*stat = EXIT_OK;
}

sig_atomic_t	runner_cmd_paren(t_runner_data *run_data)
{
	const t_command	cmd = run_data->cmd;
	pid_t			*pid;
	sig_atomic_t	stat;

	ft_assert(cmd->type == CMD_PAREN, "expected only cmd_paren");
	runner_cmd_paren_init(run_data, cmd, &pid, &stat);
	*pid = fork();
	if (*pid < 0)
		exit(EXIT_FORK_FAIL);
	if (*pid == 0)
		runner_cmd_paren_child(run_data, &stat, pid);
	command_close_ios(cmd);
	ft_arraylist_destroy(run_data->pids);
	run_data->pids = ft_arraylist_pop(run_data->backup_pids);
	run_data->pids = ft_arraylist_add(run_data->pids, pid);
	return (stat);
}

static void	runner_cmd_paren_child(
	t_runner_data *run_data,
	sig_atomic_t *stat,
	pid_t *pid)
{
	rl_clear_history();
	run_data->is_main = 0;
	free(pid);
	pid = NULL;
	*stat = runner_cmd(run_data, FORK_NOT);
	*stat = *stat << 8;
	runner_wait(run_data, stat);
	*stat = runner_normalize_status(*stat);
	runner_cmd_simple_exit_status(run_data, *stat);
}
