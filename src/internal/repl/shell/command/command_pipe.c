/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:18:39 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/20 16:10:18 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"
#include "ft_util.h"
#include "ft_memlib.h"

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
	command_free(cmd);
}
