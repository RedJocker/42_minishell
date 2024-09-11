/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 19:15:08 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/10 12:37:02 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "ft_string.h"
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal/command/command.h"
#include "internal/token/token.h"
#include "internal/command/command_internal.h"
#include "internal/command/io_handler.h"

int	token_type_is_redirect(t_token *token)
{
	return (token &&
			(token->type == OP_REDIRECT_OUT_TRUNC
			|| token->type == OP_REDIRECT_OUT_APPND
			|| token->type == OP_REDIRECT_IN));
}

int	token_type_is_word(t_token *token)
{
	return (token && token->type == WORD);
}

int	command_simple_is_invalid(
	t_token **tokens, int fst_token_idx, int *endtoken_idx)
{
	int	i;

	i = fst_token_idx - 1;
	while (++i < *endtoken_idx)
	{
		if (token_type_is_redirect(tokens[i])
			&& !token_type_is_word(tokens[i + 1]))
		{
			*endtoken_idx = i + 1;
			return (1);
		}
	}
	return (0);
}

void	command_simple_fill(
	t_command cmd, t_token **tokens, int fst_token_idx, int endtoken_idx)
{
	int	i;
	int	j;
	int	flags_mode[2];

	i = fst_token_idx - 1;
	j = 0;
	while (++i < endtoken_idx)
	{
		if (tokens[i]->type == WORD)
			cmd->simple->cmd_argv[j++] = ft_strdup(tokens[i]->content);
		else if (tokens[i]->type == OP_REDIRECT_OUT_TRUNC
				 || tokens[i]->type == OP_REDIRECT_OUT_APPND)
		{
			flags_mode[0] = O_WRONLY | O_CREAT;
			if (tokens[i]->type == OP_REDIRECT_OUT_TRUNC)
				flags_mode[0] |= O_TRUNC;
			else
				flags_mode[0] |= O_APPEND;
			flags_mode[1] = 0666;
			io_handlers_add_path(
				&cmd->io_handlers, tokens[++i]->content, flags_mode, IO_OUT);
			continue ;
		}
		else if (tokens[i]->type == OP_REDIRECT_IN)
		{
			flags_mode[0] = O_RDONLY;
			flags_mode[1] = 0666;
			io_handlers_add_path(
				&cmd->io_handlers, tokens[++i]->content, flags_mode, IO_IN);
			continue ;
		}
	}
}

t_command	command_simple_new(
	t_token **tokens, int fst_token_idx, int endtoken_idx)
{
	t_command	cmd;
	int			i;

	cmd = command_new(CMD_SIMPLE, "CMD_SIMPLE");
	if (!cmd)
		return (NULL);
	cmd->simple = ft_calloc(1, sizeof(t_command_simple));
	if (!cmd->simple)
		return (ft_free_retnull(cmd));
	i = fst_token_idx - 1;
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
	command_simple_fill(cmd, tokens, fst_token_idx, endtoken_idx);
	return (cmd);
}

void	command_simple_destroy(t_command cmd)
{
	ft_strarr_free(cmd->simple->cmd_argv);
	if (cmd->simple->cmd_path)
		free(cmd->simple->cmd_path);
	free(cmd->simple);
	command_free(cmd);
}
