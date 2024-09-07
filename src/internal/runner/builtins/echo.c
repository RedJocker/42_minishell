/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:55:24 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/07 04:59:56 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_assert.h"
#include "ft_string.h"
#include "internal/command/command.h"
#include "internal/command/io_handler.h"

int	runner_cmd_builtin_echo(t_command cmd)
{
	char			*arg;
	int				len_arg;
	int				i;
	int             out_fd;
	int				out_len;
	const char		*space_newline = " \n";

	out_fd = STDOUT;
	out_len = ft_arraylist_len(cmd->output);
	if (out_len)
	{
		if (!io_handlers_to_fd(cmd->output, &arg))
		{
			ft_assert(0, arg); // TODO handle error on redirect like cmd_simple
			return (2);
		}
		out_fd = ((t_io_handler *)
			ft_arraylist_get(cmd->output, out_len - 1))->fd;
	}
	i = 0;
	while (++i < cmd->simple->cmd_argc - 1)
	{
		arg = cmd->simple->cmd_argv[i];
		len_arg = ft_strlen(arg);
		write(out_fd, arg, len_arg);
		write(out_fd, &(space_newline[0]), 1);
	}
	if (i < cmd->simple->cmd_argc)
	{
		arg = cmd->simple->cmd_argv[i];
		len_arg = ft_strlen(arg);
		write(out_fd, arg, len_arg);
	}
	write(out_fd, &(space_newline[1]), 1);
	if (out_fd != STDOUT)
		close(out_fd);
	return (0);
}

