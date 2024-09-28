/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_invalid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 20:08:23 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/27 21:08:46 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memlib.h"
#include "ft_string.h"
#include "internal/repl/shell/command/command_internal.h"

t_command	command_invalid_new(char *message, int status)
{
	t_command	cmd;

	cmd = command_new(CMD_INVALID, "CMD_INVALID");
	if (!cmd)
		return (NULL);
	cmd->invalid = ft_calloc(1, sizeof(t_command_invalid));
	if (!cmd->invalid)
	{
		command_free(cmd);
		return (NULL);
	}
	cmd->invalid->msg = ft_strdup(message);
	if (!cmd->invalid->msg)
	{
		free(cmd->invalid);
		command_free(cmd);
		return (NULL);
	}
	cmd->invalid->status = status;
	return (cmd);
}

void	command_invalid_destroy(t_command cmd)
{
	free(cmd->invalid->msg);
	free(cmd->invalid);
	command_free(cmd);
}
