/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:55:24 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/09 04:12:31 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include "ft_assert.h"
#include "ft_string.h"
#include "internal/command/command.h"
#include "internal/command/io_handler.h"
#include "internal/default.h"

static int	set_out_len(t_command cmd, char **arg, int *out_fd);
static void	echo_display(t_command cmd, char **arg, int *out_fd);

int	runner_cmd_builtin_echo(t_command cmd)
{
	char	*arg;
	int		out_fd;

	out_fd = STDOUT_FILENO;
	if (!set_out_len(cmd, &arg, &out_fd))
		return (STDERR_FILENO);
	echo_display(cmd, &arg, &out_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	return (0); // TODO:Verifiy which name can be used to remove magic number.
}

static int	set_out_len(t_command cmd, char **arg, int *out_fd)
{
	int	out_len;

	out_len = ft_arraylist_len(cmd->output);
	if (out_len)
	{
		if (!io_handlers_to_fd(cmd->output, arg))
		{
			ft_assert(0, *arg); // TODO: handle error on redirect like cmd_simple
			return (FAIL);
		}
		*out_fd = ((t_io_handler *)
			ft_arraylist_get(cmd->output, out_len - 1))->fd; // TODO:Verifiy which name can be used to remove magic number.
	}
	return (true);
}

static void	echo_display(t_command cmd, char **arg, int *out_fd)
{
	int				len_arg;
	int				i;
	const char		*space_newline = " \n";

	i = DEFAULT;
	len_arg = DEFAULT;
	while (++i < cmd->simple->cmd_argc - 1) // TODO:Verifiy which name can be used to remove magic number.
	{
		*arg = cmd->simple->cmd_argv[i];
		len_arg = ft_strlen(*arg);
		write(*out_fd, *arg, len_arg);
		write(*out_fd, &(space_newline[0]), STDOUT_FILENO); // TODO:Verifiy which name can be used to remove magic number.
	}
	if (i < cmd->simple->cmd_argc)
	{
		*arg = cmd->simple->cmd_argv[i];
		len_arg = ft_strlen(*arg);
		write(*out_fd, *arg, len_arg);
	}
	write(*out_fd, &(space_newline[1]), STDOUT_FILENO); // TODO:Verifiy which name can be used to remove magic number.
}
