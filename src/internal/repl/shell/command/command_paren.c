/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_paren.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:11:17 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 23:16:48 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memlib.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"

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

void	command_paren_add_pipe_io(
	t_command cmd, int pipe_fd, t_io_direction dir)
{
	command_add_pipe_io(cmd->paren->cmd, pipe_fd, dir);
}

void	command_paren_close_ios(t_command cmd)
{
	io_handlers_close(cmd->io_handlers);
	command_close_ios(cmd->paren->cmd);
}
