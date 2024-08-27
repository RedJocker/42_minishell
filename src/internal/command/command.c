/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 01:11:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/27 00:12:39 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "ft_memlib.h"
#include "command_internal.h"

int	command_debug_id(void)
{
	static int	id = 0;

	return (id++);
}

int	command_token_precedence(t_token_type token_type)
{
	if (token_type == WORD)
		return (10);
	else if (token_type == OP_PIPE)
		return (9);
	return (11);
}

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
	if (cmd->output.type == IO_PATH)
		free(cmd->output.path);
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
	cmd->output.type = IO_NONE;
	id_str = ft_itoa(command_debug_id());
	if (!id_str)
		return (ft_free_retnull(cmd));
	cmd->debug_id = ft_strjoin(type_str, id_str);
	free(id_str);
	return (cmd);
}
