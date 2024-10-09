/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:55:24 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/08 06:17:41 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "ft_string.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"

// TODO handle echo -n case
sig_atomic_t	runner_cmd_builtin_echo(t_command cmd)
{
	char		*arg;
	int			len_arg;
	int			i;
	const char	*space_newline = " \n";

	i = DEFAULT;
	len_arg = DEFAULT;
	while (++i < cmd->simple->cmd_argc - CMD_NAME) 
	{
		arg = cmd->simple->cmd_argv[i];
		len_arg = ft_strlen(arg);
		write(STDOUT_FILENO, arg, len_arg);
		write(STDOUT_FILENO, &(space_newline[DEFAULT]), CHAR_BYTE);
	}
	if (i < cmd->simple->cmd_argc)
	{
		arg = cmd->simple->cmd_argv[i];
		len_arg = ft_strlen(arg);
		write(STDOUT_FILENO, arg, len_arg);
	}
	write(STDOUT_FILENO, &(space_newline[DEFAULT_BEGIN]), CHAR_BYTE);
	return (EXIT_OK);
}
