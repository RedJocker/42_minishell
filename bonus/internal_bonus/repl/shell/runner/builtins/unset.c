/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 01:58:35 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/27 19:01:27 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/env/env.h"
#include "internal_bonus/repl/shell/command/command.h"
#include "internal_bonus/repl/shell/runner/builtins/builtins.h"

static sig_atomic_t	unset_cmds(t_command cmd, sig_atomic_t status);

sig_atomic_t	runner_cmd_builtin_unset(t_command cmd)
{
	sig_atomic_t	status;

	status = EXIT_OK;
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		return (status);
	else
		status = unset_cmds(cmd, status);
	return (status);
}

static sig_atomic_t	unset_cmds(t_command cmd, sig_atomic_t status)
{
	int		i;
	char	*error_msg;

	i = DEFAULT;
	while (cmd->simple->cmd_argv[++i])
	{
		error_msg = NULL;
		if (is_identifier(cmd->simple->cmd_argv[i]))
			env_destroy_single_var(cmd->simple->cmd_argv[i]);
		else
			ft_asprintf(&error_msg, \
				"minishell: unset: `%s': not a valid identifier\n", \
				cmd->simple->cmd_argv[i]);
		if (error_msg)
		{
			status = EXIT_FAILURE;
			write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
			free(error_msg);
		}
	}
	return (status);
}
