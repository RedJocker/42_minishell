/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 01:11:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/01 16:58:25 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "ft_string.h"
#include "ft_memlib.h"
#include "command_internal.h"
#include "ft_util.h"
#include "internal/token/token.h"
#include "io_handler.h"

void	command_destroy(t_command cmd)
{
	if (cmd->type == CMD_SIMPLE)
		command_simple_destroy(cmd);
	else if (cmd->type == CMD_INVALID)
		command_invalid_destroy(cmd);
}

int	command_operator_idx(t_token **tokens, int tokens_len)
{
	int	cmd_operator_idx;
	int	precedence_cur;
	int	precedence_next;
	int	i;

	precedence_cur = command_token_precedence(WORD);
	cmd_operator_idx = -1;
	i = -1;
	while (++i < tokens_len)
	{
		precedence_next = command_token_precedence(tokens[i]->type);
		if (precedence_next < precedence_cur)
		{
			cmd_operator_idx = i;
			precedence_cur = precedence_next;
		}
	}
	return (cmd_operator_idx);
}

void	command_free(t_command cmd)
{
	ft_arraylist_destroy(cmd->output);
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
	cmd->output = ft_arraylist_new((t_consumer) io_handler_destroy);
	id_str = ft_itoa(command_debug_id());
	if (!id_str)
		return (ft_free_retnull(cmd));
	cmd->debug_id = ft_strjoin(type_str, id_str);
	free(id_str);
	return (cmd);
}