/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 01:58:35 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:06:20 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "internal_bonus/default.h"
#include "internal_bonus/env/env.h"
#include "internal_bonus/repl/shell/command/command.h"

sig_atomic_t	runner_cmd_builtin_unset(t_command cmd)
{
	int				i;
	sig_atomic_t	status;

	i = DEFAULT;
	status = EXIT_OK;
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		return (status);
	else
		while (cmd->simple->cmd_argv[++i])
			env_destroy_single_var(cmd->simple->cmd_argv[i]);
	return (status);
}
