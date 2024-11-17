/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_eof.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 17:18:25 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:09:19 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_bonus/repl/shell/command/command_internal.h"

t_command	command_eof_new(void)
{
	t_command	cmd;

	cmd = command_new(CMD_EOF, "CMD_EOF");
	if (!cmd)
		return (NULL);
	return (cmd);
}

void	command_eof_destroy(t_command cmd)
{
	command_free(cmd);
}
