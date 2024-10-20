/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 05:02:21 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/20 05:27:24 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include "ft_string.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"

sig_atomic_t	runner_cmd_builtin_exit(t_command cmd)
{
	// int				i;
	sig_atomic_t	status;

	// i = DEFAULT;
	status = EXIT_OK;
	write(STDOUT_FILENO, "exit\n", ft_strlen("exit\n"));
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		exit(status);
	// else
	// 	while (cmd->simple->cmd_argv[++i])
	// 		if (export_args(cmd->simple->cmd_argv[i], status) == EXIT_FAILURE)
	// 			status = EXIT_FAILURE;
	return (status);
}
