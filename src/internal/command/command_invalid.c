/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_invalid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 20:08:23 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/25 22:01:35 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_internal.h"
#include "ft_memlib.h"
#include "ft_string.h"
#include "internal/command/command.h"
#include "minishell.h"

t_command	command_invalid_new(char *message)
{
	t_command	cmd;

	command_new(CMD_INVALID, "CMD_INVALID");
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
	return (cmd);
}

void	command_invalid_destroy(t_command cmd)
{
	free(cmd->invalid->msg);
	free(cmd->invalid);
}
