/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 01:11:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/13 22:38:41 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "ft_string.h"
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/repl/shell/token/token.h"

void	command_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir)
{
	if (cmd->type == CMD_SIMPLE)
		command_simple_add_pipe_io(cmd, pipe_fd, dir);
	else if (cmd->type == CMD_PIPE)
		command_pipe_add_pipe_io(cmd, pipe_fd, dir);
	else if (cmd->type == CMD_AND)
		command_and_add_pipe_io(cmd, pipe_fd, dir);
	else if (cmd->type == CMD_OR)
		command_or_add_pipe_io(cmd, pipe_fd, dir);
	else if (cmd->type == CMD_PAREN)
		command_paren_add_pipe_io(cmd, pipe_fd, dir);
}

void	command_close_ios(t_command cmd)
{
	if (cmd->type == CMD_SIMPLE)
		command_simple_close_ios(cmd);
	else if (cmd->type == CMD_PIPE)
		command_pipe_close_ios(cmd);
	else if (cmd->type == CMD_AND)
		command_and_close_ios(cmd);
	else if (cmd->type == CMD_OR)
		command_or_close_ios(cmd);
	else if (cmd->type == CMD_PAREN)
		command_paren_close_ios(cmd);
}

void	command_destroy(t_command cmd)
{
	if (cmd->type == CMD_SIMPLE)
		command_simple_destroy(cmd);
	else if (cmd->type == CMD_PIPE)
		command_pipe_destroy(cmd);
	else if (cmd->type == CMD_AND)
		command_and_destroy(cmd);
	else if (cmd->type == CMD_OR)
		command_or_destroy(cmd);
	else if (cmd->type == CMD_PAREN)
		command_paren_destroy(cmd);
	else if (cmd->type == CMD_EOF)
		command_eof_destroy(cmd);
	else if (cmd->type == CMD_INVALID)
		command_invalid_destroy(cmd);
}

void	command_free(t_command cmd)
{
	ft_arraylist_destroy(cmd->io_handlers);
	free(cmd->debug_id);
	free(cmd);
}

t_command	command_new(t_command_type	type, char *type_str)
{
	t_command	cmd;
	char		*id_str;

	cmd = ft_calloc(1, sizeof(struct s_command));
	if (!cmd)
		return (NULL);
	cmd->type = type;
	cmd->io_handlers = ft_arraylist_new((t_consumer) io_handler_destroy);
	id_str = ft_itoa(command_debug_id());
	if (!id_str)
		return (ft_free_retnull(cmd));
	cmd->debug_id = ft_strjoin(type_str, id_str);
	free(id_str);
	return (cmd);
}
