/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 19:15:08 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 23:15:27 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/command_internal.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/repl/shell/token/token.h"

void	command_simple_add_pipe_io(
		t_command cmd, int pipe_fd, t_io_direction dir)
{
	io_handlers_add_pipe(&cmd->io_handlers, pipe_fd, dir);
}

int	command_simple_is_invalid(
	t_token **tokens, int *endtoken_idx)
{
	int	i;

	i = -1;
	while (++i < *endtoken_idx)
	{
		if (token_type_is_redirect(tokens[i])
			&& !token_type_is_word(tokens[i + 1]))
		{
			*endtoken_idx = i + 1;
			return (1);
		}
		else if (tokens[i]->type == INVALID)
		{
			*endtoken_idx = i;
			return (1);
		}
	}
	return (0);
}

t_command	command_simple_new(t_token **tokens, int endtoken_idx)
{
	t_command	cmd;
	int			i;

	cmd = command_new(CMD_SIMPLE, "CMD_SIMPLE");
	if (!cmd)
		return (NULL);
	cmd->simple = ft_calloc(1, sizeof(t_command_simple));
	if (!cmd->simple)
		return (ft_free_retnull(cmd));
	i = -1;
	while (++i < endtoken_idx - 1)
	{
		if (token_type_is_redirect(tokens[i])
			&& token_type_is_word(tokens[i + 1]))
			++i;
		else if (token_type_is_word(tokens[i]))
			cmd->simple->cmd_argc++;
	}
	cmd->simple->cmd_argv = (
			ft_calloc(cmd->simple->cmd_argc + 1, sizeof(char *)));
	command_simple_fill(cmd, tokens, endtoken_idx);
	return (cmd);
}

void	command_simple_destroy(t_command cmd)
{
	ft_strarr_free(cmd->simple->cmd_argv);
	if (cmd->simple->cmd_envp)
		ft_strarr_free(cmd->simple->cmd_envp);
	if (cmd->simple->cmd_path)
		free(cmd->simple->cmd_path);
	free(cmd->simple);
	command_free(cmd);
}

void	command_simple_close_ios(t_command cmd)
{
	io_handlers_close(cmd->io_handlers);
}
