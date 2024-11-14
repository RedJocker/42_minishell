/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_or.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:10:09 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/13 22:41:31 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "command_internal.h"
#include "ft_memlib.h"
#include "ft_assert.h"
#include <unistd.h>

t_command	command_or_new(t_command cmd_before, t_command cmd_after)
{
	t_command	cmd;

	cmd = command_new(CMD_OR, "CMD_OR");
	if (!cmd)
		return (NULL);
	cmd->or = ft_calloc(1, sizeof(t_command_or));
	if (!cmd->or)
		return (ft_free_retnull(cmd));
	cmd->or->cmd_before = cmd_before;
	cmd->or->cmd_after = cmd_after;
	return (cmd);
}

void	command_or_destroy(t_command cmd)
{
	command_destroy(cmd->or->cmd_before);
	command_destroy(cmd->or->cmd_after);
	free(cmd->or);
	command_free(cmd);
}

void	command_or_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir)
{
	int	fd;

	if (dir == IO_IN)
		command_add_pipe_io(cmd->or->cmd_before, pipe_fd, dir);
	else if (dir == IO_OUT)
	{
		fd = dup(pipe_fd);
		command_add_pipe_io(cmd->or->cmd_before, fd, dir);
		command_add_pipe_io(cmd->or->cmd_after, pipe_fd, dir);
	}
	else
		ft_assert(0, "unexpected io_direction command_or_add_pipe_io");
}

void	command_or_close_ios(t_command cmd)
{
	io_handlers_close(cmd->io_handlers);
	command_close_ios(cmd->or->cmd_before);
	command_close_ios(cmd->or->cmd_after);
}
