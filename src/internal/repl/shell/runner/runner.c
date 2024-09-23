/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:38:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/23 09:50:36 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "collection/ft_arraylist.h"
#include "ft_assert.h"
#include "ft_ctype.h"
#include "ft_memlib.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "ft_util.h"
#include "stringbuilder.h"
#include "internal/env/envp.h"
#include "internal/env/env.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/repl/shell/runner/runner.h"
#include "internal/repl/shell/runner/builtins/builtins.h"
#include "internal/ft_extension.h"

int runner_cmd(t_command cmd, t_arraylist *pids, int last_cmd_status);

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

sig_atomic_t	runner_cmd_builtin(t_builtin builtin, t_command cmd, t_arraylist *pids)
{
	(void) pids;
	if (builtin == BUILTIN_ECHO)
		runner_cmd_builtin_echo(cmd);
	// TODO: check need for return status_code 
	exit(0);
	return (0);
}

void runner_cmd_simple_panic(t_command cmd, char *msg, sig_atomic_t status_code)
{

	ft_puterrl(msg);
	free(msg);
	command_destroy(cmd);
	exit(status_code);
}

sig_atomic_t	runner_cmd_simple_execve(t_command cmd)
{
	sig_atomic_t	status_code;
	char			*err_msg;

	execve(cmd->simple->cmd_path, cmd->simple->cmd_argv, cmd->simple->cmd_envp);
	status_code = 1;
	err_msg = ft_strdup("TODO: make a error message and check better status code");
	runner_cmd_simple_panic(cmd, err_msg, status_code);
	return (status_code);
}

void command_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir);

// pipe_fds[0] read, pipe_fds[1] write
sig_atomic_t	runner_cmd_pipe(t_command cmd, t_arraylist *pids, sig_atomic_t last_status_code)
{
	sig_atomic_t	status;
	int				pipe_fds[2];

	ft_assert(cmd->type == CMD_PIPE, "expected only cmd_pipe");
	status = 0;
	pipe(pipe_fds);
	command_add_pipe_io(cmd->pipe->cmd_before, pipe_fds[1], IO_OUT);
	command_add_pipe_io(cmd->pipe->cmd_after, pipe_fds[0], IO_IN);
	runner_cmd(cmd->pipe->cmd_before, pids, last_status_code);
	runner_cmd(cmd->pipe->cmd_after, pids, last_status_code);
	return (status);
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
	status = 0;
	if (*pid < 0)
		exit(1);
	if (*pid == 0)
	{
		free(pid);
		cmd->simple->cmd_envp = __environ; // TODO: change to get_envp
		cmd->simple->cmd_path = (
				envp_find_bin_by_name(cmd->simple->cmd_argv[0], cmd->simple->cmd_envp));
		ft_arraylist_destroy(*pids);
		if (!io_handlers_redirect(cmd->io_handlers, &err_msg))
			runner_cmd_simple_panic(cmd, ft_strdup(err_msg), 1);
		maybe_builtin = runner_maybe_cmd_builtin(cmd);
		if (maybe_builtin)
		{
			status = runner_cmd_builtin(maybe_builtin, cmd, pids);
			return (status);
		}
		runner_cmd_simple_execve(cmd);
	}
	else
	{
		//TODO: change status to function error handling
		*pids = ft_arraylist_add(*pids, pid);
		io_handlers_close(cmd->io_handlers);
		if (!(*pids))
			status = 1;
	}
	return (status);
}

sig_atomic_t	runner_cmd_invalid(t_command cmd, t_arraylist *pids)
{
	(void) pids;
	ft_assert(cmd->type == CMD_INVALID, "expected only invalid");
	ft_puterrl(cmd->invalid->msg);
	return (2);
}

char	*env_mock(char *s)
{
	(void) s;
	return (ft_strdup("abc def"));
}

int	runner_cmd_expand_dollar(char *str, t_stringbuilder *builder, \
		sig_atomic_t last_status_code)
{
	int		i;
	char	*var_name;
	char	*var_value;

	ft_assert(ft_strncmp(str, "$", 1) == 0, "expected $");
	if (ft_strncmp(str, "$?", 2) == 0)
	{
		var_value = ft_itoa(last_status_code);
		stringbuilder_addstr(builder, var_value);
		free(var_value);
		return (1);
	}
	i = 1;
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		i++;
		while (ft_isalnum(str[i]) || str[i] == '_')
			i++;
	}
	var_name = ft_calloc(i, sizeof(char));
	ft_strlcpy(var_name, str + 1, i);
	// TODO: look expansion on env
	var_value = env_mock(var_name);
	free(var_name);
	stringbuilder_addstr(builder, var_value);
	free(var_value);
	return (i - 1);
}

int	runner_cmd_expand_dollar_split(char *str, t_stringbuilder *builder, \
		t_arraylist *lst_new_args, sig_atomic_t last_status_code)
{
	int		i;
	char	**split;
	char	*temp;
	int		j;

	i = runner_cmd_expand_dollar(str, builder, last_status_code);
	temp = stringbuilder_build(*builder);
	split = ft_splitfun(temp, (t_pred_int) ft_isspace);
	free(temp);
	j = -1;
	while (split[++j + 1])
		*lst_new_args = ft_arraylist_add(*lst_new_args, ft_strdup(split[j]));
	*builder = stringbuilder_new();
	stringbuilder_addstr(builder, split[j]);
	ft_strarr_free(split);
	return (i);
}

void	runner_cmd_expand_str(char *str, sig_atomic_t last_status_code, \
			t_arraylist *lst_new_args)
{
	t_stringbuilder	builder;
	char			*res;
	char			open_quote;
	int				i;

	builder = stringbuilder_new();
	open_quote = 0;
	i = -1;
	while (str[++i])
	{
		if (open_quote == 0)
		{
			if (str[i] == '$')
				i += runner_cmd_expand_dollar_split(str + i, &builder, lst_new_args, last_status_code);
			else if (str[i] == '\'' || str[i] == '\"')
				open_quote = str[i];
			else
				builder = stringbuilder_addchar(builder, str[i]);
		}
		else
		{
			if (str[i] == open_quote)
				open_quote = 0;
			else if (open_quote == '\"' && str[i] == '$')
				i += runner_cmd_expand_dollar(str + i, &builder, last_status_code);
			else
				builder = stringbuilder_addchar(builder, str[i]);
		}
	}
	res = stringbuilder_build(builder);
	*lst_new_args = (ft_arraylist_add(*lst_new_args, res));
}

void	runner_cmd_expand(t_command cmd, sig_atomic_t last_status_code)
{
	t_arraylist	lst_new_args;
	int			i;

	if (cmd->type != CMD_SIMPLE)
		return ;
	lst_new_args = ft_arraylist_new(free);
	i = -1;
	while (cmd->simple->cmd_argv[++i])
		runner_cmd_expand_str(
			cmd->simple->cmd_argv[i], last_status_code, &lst_new_args);
	// TODO: improve allocation error handling
	ft_strarr_free(cmd->simple->cmd_argv);
	cmd->simple->cmd_argv = ft_lststr_to_arrstr(lst_new_args);
	ft_arraylist_destroy(lst_new_args);
	cmd->simple->cmd_argc = 0;
	while (cmd->simple->cmd_argv[cmd->simple->cmd_argc])
		cmd->simple->cmd_argc++;
}

sig_atomic_t runner_cmd(t_command cmd, t_arraylist *pids, sig_atomic_t last_cmd_status)
{
	sig_atomic_t	status;

	runner_cmd_expand(cmd, last_cmd_status);
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
