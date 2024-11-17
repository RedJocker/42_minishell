/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:55:24 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:06:53 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/repl/shell/command/command.h"

static bool	is_option(t_command cmd);

sig_atomic_t	runner_cmd_builtin_echo(t_command cmd)
{
	int			i;
	bool		should_print_newline;
	const char	*space_newline = " \n";

	should_print_newline = is_option(cmd);
	i = DEFAULT + !should_print_newline;
	while (++i < cmd->simple->cmd_argc - CMD_NAME)
	{
		write(STDOUT_FILENO, cmd->simple->cmd_argv[i], \
		ft_strlen(cmd->simple->cmd_argv[i]));
		write(STDOUT_FILENO, &(space_newline[DEFAULT]), CHAR_BYTE);
	}
	if (i < cmd->simple->cmd_argc)
		write(STDOUT_FILENO, cmd->simple->cmd_argv[i], \
		ft_strlen(cmd->simple->cmd_argv[i]));
	if (should_print_newline)
		write(STDOUT_FILENO, &(space_newline[DEFAULT_BEGIN]), CHAR_BYTE);
	return (EXIT_OK);
}

static bool	is_option(t_command cmd)
{
	int	i;

	i = CHAR_BYTE;
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		return (true);
	else if (strncmp(cmd->simple->cmd_argv[CHAR_BYTE], "-n", \
		CHAR_BYTE + CHAR_BYTE) == DEFAULT)
	{
		while (cmd->simple->cmd_argv[CHAR_BYTE][i])
			if (cmd->simple->cmd_argv[CHAR_BYTE][i++] != 'n')
				return (true);
		return (false);
	}
	return (true);
}
