/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:38:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/18 00:27:39 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include "internal/repl/command/command.h"
#include "internal/repl/command/io_handler.h"
#include "internal/repl/runner/runner.h"
#include "internal/repl/runner/builtins/builtins.h"
#include "internal/ft_extension.h"

int	runner_cmd_simple(t_command cmd, t_arraylist *pids)
{
	pid_t	*pid;
	int		status;
	char	*err_msg;
	char	**envp;

	envp = get_envp();
	ft_assert(cmd->type == CMD_SIMPLE, "expected only cmd_simple");
	pid = malloc(sizeof(pid_t));
	*pid = fork();
	if (*pid < 0)
		exit(1);
	if (*pid == 0)
	{
		free(pid);
		cmd->simple->cmd_path = (
				get_bin_path_with_envp(cmd->simple->cmd_argv[0], envp));
		if (!io_handlers_redirect(cmd->io_handlers, &err_msg))
		{
			ft_strarr_free(envp);
			ft_puterrl(err_msg);
			status = 1;
			// todo check possible leak pids list
			command_destroy(cmd);
			exit(status);
		}
		execve(cmd->simple->cmd_path, cmd->simple->cmd_argv, envp);
		ft_strarr_free(envp);
		// TODO: check possible leak pids list
		status = 1;
		command_destroy(cmd);
		exit(status);
	}
	else
	{
		//TODO: change status to function error handling
		*pids = ft_arraylist_add(*pids, pid);
		if (!(*pids))
			status = 1;
		else
			status = 0;
		ft_strarr_free(envp);
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

char	*env_mock(char *s)
{
	(void) s;
	return (ft_strdup("abc def"));
}

int	runner_cmd_expand_dollar(
	char *str, t_stringbuilder *builder, int last_status_code)
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

int	runner_cmd_expand_dollar_split(
	char *str,
	t_stringbuilder *builder,
	t_arraylist *lst_new_args,
	int last_status_code)
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
	stringbuilder_addstr(builder, ft_strdup(split[j]));
	ft_strarr_free(split);
	return (i);
}

void	runner_cmd_expand_str(
	char *str, int last_status_code, t_arraylist *lst_new_args)
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

void	runner_cmd_expand(t_command cmd, int last_status_code)
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
