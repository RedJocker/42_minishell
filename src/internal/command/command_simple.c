/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 19:15:08 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/30 20:55:36 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal/token/token.h"
#include "internal/command/command_internal.h"
#include "io_handler.h"
#include <fcntl.h>

int	token_type_is_redirect(t_token *token)
{
	return (token && token->type == OP_REDIRECT_OUT_TRUNC);
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

	i = fst_token_idx - 1;
	j = 0;
	while (++i < endtoken_idx)
	{
		if (tokens[i]->type == WORD)
			cmd->simple->cmd_argv[j++] = ft_strdup(tokens[i]->content);
		else if (tokens[i]->type == OP_REDIRECT_OUT_TRUNC)
		{
			io_handlers_add_path(
				&cmd->output, tokens[++i]->content,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
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
	while (++i < endtoken_idx)
		cmd->simple->cmd_argc++;
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
