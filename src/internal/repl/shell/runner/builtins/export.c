/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 00:46:01 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/25 00:48:10 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include "ft_string.h"
#include "ft_stdio.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/io_handler.h"

static int	set_out_len(t_command cmd, char **arg, int *out_fd);
static void	export_display(t_command cmd, char **arg, int *out_fd);

int	runner_cmd_builtin_export(t_command cmd)
{
	char	*arg;
	int		out_fd;

	out_fd = STDOUT_FILENO;
	if (!set_out_len(cmd, &arg, &out_fd))
	{
		command_destroy(cmd);
		return (2); // FIX: Is this status code?
	}
	export_display(cmd, &arg, &out_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	return (0); // TODO:Verifiy which name can be used to remove magic number.
}

static int	set_out_len(t_command cmd, char **arg, int *out_fd)
{
	int	out_len;

	out_len = ft_arraylist_len(cmd->io_handlers);
	if (out_len)
	{
		if (!io_handlers_to_fd(cmd->io_handlers, arg))
			return (FAIL);
		*out_fd = ((t_io_handler *)
				ft_arraylist_get(cmd->io_handlers, out_len - 1))->fd;
	}
	return (true);
}

static void	export_display(t_command cmd, char **arg, int *out_fd)
{
	char	*argc_value;

	ft_asprintf(&argc_value, "%d", cmd->simple->cmd_argc);
	write(*out_fd, "argc - ", ft_strlen("argc - "));
	write(*out_fd, argc_value, ft_strlen(argc_value));
	write(*out_fd, "\n", ft_strlen("\n"));
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
