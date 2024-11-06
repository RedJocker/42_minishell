/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:18:39 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/05 02:25:05 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"
#include "ft_memlib.h"
#include "ft_assert.h"

// TODO: Move to command_paren.c
t_command	command_paren_new(t_command cmd_inside)
{
	t_command	cmd;

	cmd = command_new(CMD_PAREN, "CMD_PAREN");
	if (!cmd)
		return (NULL);
	cmd->paren = ft_calloc(1, sizeof(t_command_paren));
	if (!cmd->paren)
		return (ft_free_retnull(cmd_inside));
	cmd->paren->cmd = cmd_inside;
	return (cmd);
}

void	command_paren_destroy(t_command cmd)
{
	command_destroy(cmd->paren->cmd);
	free(cmd->paren);
	command_free(cmd);
}


// TODO: Move to command_and.c
t_command	command_and_new(t_command cmd_before, t_command cmd_after)
{
	t_command	cmd;

	cmd = command_new(CMD_AND, "CMD_AND");
	if (!cmd)
		return (NULL);
	cmd->pipe = ft_calloc(1, sizeof(t_command_and));
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

// TODO: Move to command_or.c
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

void	command_and_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir)
{
	if (dir == IO_IN)
		command_add_pipe_io(cmd->and->cmd_before, pipe_fd, dir);
	else if (dir == IO_OUT)
		command_add_pipe_io(cmd->and->cmd_after, pipe_fd, dir);
	else
		ft_assert(0, "unexpected io_direction command_and_add_pipe_io");
}

void	command_or_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir)
{
	if (dir == IO_IN)
		command_add_pipe_io(cmd->or->cmd_before, pipe_fd, dir);
	else if (dir == IO_OUT)
		command_add_pipe_io(cmd->or->cmd_after, pipe_fd, dir);
	else
		ft_assert(0, "unexpected io_direction command_or_add_pipe_io");
}

void	command_paren_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir)
{
	command_add_pipe_io(cmd->paren->cmd, pipe_fd, dir);
}

