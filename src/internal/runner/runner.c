/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:38:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/09 05:27:51 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include "collection/ft_arraylist.h"
#include "ft_assert.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "ft_util.h"
#include "internal/envp.h"
#include "internal/command/command.h"
#include "internal/command/io_handler.h"
#include "internal/runner/runner.h"
#include "internal/runner/builtins/builtins.h"

int	runner_cmd_simple(t_command cmd, t_arraylist *pids)
{
	pid_t	*pid;
	int      status;
	char	*err_msg;

	ft_assert(cmd->type == CMD_SIMPLE, "expected only cmd_simple");
	pid = malloc(sizeof(pid_t));
	*pid = fork();
	if (*pid < 0)
		exit(1);
	if (*pid == 0)
	{
		free(pid);
		cmd->simple->cmd_path = (
				envp_find_bin_by_name(cmd->simple->cmd_argv[0], __environ));
		if (!io_handlers_redirect(cmd->output, STDOUT_FILENO, &err_msg))
		{
			ft_putendl(err_msg); // TODO: Set the correct fd to write the error.
			status = 1;
			// todo check possible leak pids list
			command_destroy(cmd);
			exit(status);
		}
		execve(cmd->simple->cmd_path, cmd->simple->cmd_argv, __environ);
		// todo check possible leak pids list
		status = 1;
		command_destroy(cmd);
		exit(status);
	}
	else
	{
		//TODO change status to function error handling 
		*pids = ft_arraylist_add(*pids, pid);
		if (!(*pids))
			status = 1;
		else
			status = 0;
	}
	return (status);
}

int	runner_cmd_invalid(t_command cmd, t_arraylist *pids)
{
	(void) pids;
	ft_assert(cmd->type == CMD_INVALID, "expected only invalid");
	ft_puterrl(cmd->invalid->msg);
	return (2);
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


int	runner_cmd_builtin(t_builtin builtin, t_command cmd, t_arraylist *pids)
{
	(void) pids;
	if (builtin == BUILTIN_ECHO)
		runner_cmd_builtin_echo(cmd);
	return (0);
}

void	runner_cmd_expand_str(
	char *str, int last_status_code, t_arraylist *lst_new_args)
{
	// TODO cmd_expand on general case
	if (ft_strncmp(str, "$?", 3) == 0)
		*lst_new_args = (
			ft_arraylist_add(*lst_new_args, ft_itoa(last_status_code)));
	else
		*lst_new_args = (
			ft_arraylist_add(*lst_new_args, ft_strdup(str)));
}

void	runner_cmd_expand(t_command cmd, int last_status_code)
{
	t_arraylist lst_new_args;
	int			i;

	if (cmd->type != CMD_SIMPLE)
		return;
	lst_new_args = ft_arraylist_new(free);
	i = -1;
	while (cmd->simple->cmd_argv[++i])
		runner_cmd_expand_str(
			cmd->simple->cmd_argv[i], last_status_code, &lst_new_args);
	// TODO improve allocation error handling
	ft_strarr_free(cmd->simple->cmd_argv);
	cmd->simple->cmd_argv = ft_lststr_to_arrstr(lst_new_args);
	ft_arraylist_destroy(lst_new_args);
	cmd->simple->cmd_argc = 0;
	while (cmd->simple->cmd_argv[cmd->simple->cmd_argc])
		cmd->simple->cmd_argc++;
}

int	runner(t_command cmd, int last_cmd_status)
{
	t_arraylist	pids;
	int			pids_len;
	int			status;
	int			i;
	t_builtin	maybe_builtin;

	pids = ft_arraylist_new(free);
	runner_cmd_expand(cmd, last_cmd_status);
	//ft_strarr_printfd(cmd->simple->cmd_argv, 1);
	maybe_builtin = runner_maybe_cmd_builtin(cmd);
	if (maybe_builtin)
		status = runner_cmd_builtin(maybe_builtin, cmd, &pids);
	else if (cmd->type == CMD_SIMPLE)
		status = runner_cmd_simple(cmd, &pids);
	else if (cmd->type == CMD_INVALID)
		status = runner_cmd_invalid(cmd, &pids);
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
