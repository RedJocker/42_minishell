/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:55:24 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/01 02:34:39 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "ft_string.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/io_handler.h"


// TODO handle echo -n case
sig_atomic_t	runner_cmd_builtin_echo(t_command cmd)
{
	char		*arg;
	int			len_arg;
	int			i;
	const char	*space_newline = " \n";

	i = 0;
	len_arg = 0;
	while (++i < cmd->simple->cmd_argc - 1) 
	{
		arg = cmd->simple->cmd_argv[i];
		len_arg = ft_strlen(arg);
		write(1, arg, len_arg);
		write(1, &(space_newline[0]), 1);
	}
	if (i < cmd->simple->cmd_argc)
	{
		arg = cmd->simple->cmd_argv[i];
		len_arg = ft_strlen(arg);
		write(1, arg, len_arg);
	}
	write(1, &(space_newline[1]), 1);
	return (0);
}
