/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:18:39 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:08:49 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_assert.h"
#include "ft_memlib.h"
#include "internal_bonus/repl/shell/command/command.h"
#include "internal_bonus/repl/shell/command/command_internal.h"

t_command	command_pipe_new(t_command cmd_before, t_command cmd_after)
{
	t_command	cmd;

	cmd = command_new(CMD_PIPE, "CMD_PIPE");
	if (!cmd)
		return (NULL);
	cmd->pipe = ft_calloc(1, sizeof(t_command_pipe));
	if (!cmd->pipe)
		return (ft_free_retnull(cmd));
	cmd->pipe->cmd_before = cmd_before;
	cmd->pipe->cmd_after = cmd_after;
	return (cmd);
}

void	command_pipe_destroy(t_command cmd)
{
	command_destroy(cmd->pipe->cmd_before);
	command_destroy(cmd->pipe->cmd_after);
	free(cmd->pipe);
	command_free(cmd);
}

void	command_pipe_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir)
{
	if (dir == IO_IN)
		command_add_pipe_io(cmd->pipe->cmd_before, pipe_fd, dir);
	else if (dir == IO_OUT)
		command_add_pipe_io(cmd->pipe->cmd_after, pipe_fd, dir);
	else
		ft_assert(0, "unexpected io_direction at command_pipe_add_pipe_io");
}

void	command_pipe_close_ios(t_command cmd)
{
	io_handlers_close(cmd->io_handlers);
	command_close_ios(cmd->pipe->cmd_before);
	command_close_ios(cmd->pipe->cmd_after);
}
