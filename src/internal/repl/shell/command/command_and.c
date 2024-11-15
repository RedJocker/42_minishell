/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_and.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:08:27 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 02:28:26 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "command_internal.h"
#include "ft_memlib.h"
#include "ft_assert.h"
#include <unistd.h>

t_command	command_and_new(t_command cmd_before, t_command cmd_after)
{
	t_command	cmd;

	cmd = command_new(CMD_AND, "CMD_AND");
	if (!cmd)
		return (NULL);
	cmd->and = ft_calloc(1, sizeof(t_command_and));
	if (!cmd->and)
		return (ft_free_retnull(cmd));
	cmd->and->cmd_before = cmd_before;
	cmd->and->cmd_after = cmd_after;
	return (cmd);
}

void	command_and_destroy(t_command cmd)
{
	command_destroy(cmd->and->cmd_before);
	command_destroy(cmd->and->cmd_after);
	free(cmd->and);
	command_free(cmd);
}

void	command_and_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir)
{
	int	fd;

	if (dir == IO_IN)
		command_add_pipe_io(cmd->and->cmd_before, pipe_fd, dir);
	else if (dir == IO_OUT)
	{
		fd = dup(pipe_fd);
		command_add_pipe_io(cmd->and->cmd_before, fd, dir);
		command_add_pipe_io(cmd->and->cmd_after, pipe_fd, dir);
	}
	else
		ft_assert(0, "unexpected io_direction command_and_add_pipe_io");
}

void	command_and_close_ios(t_command cmd)
{
	io_handlers_close(cmd->io_handlers);
	command_close_ios(cmd->and->cmd_before);
	command_close_ios(cmd->and->cmd_after);
}
