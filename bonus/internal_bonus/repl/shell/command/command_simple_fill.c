/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_simple_fill.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 00:32:09 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:08:35 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "ft_string.h"
#include "internal_bonus/repl/shell/command/command.h"
#include "internal_bonus/repl/shell/command/io_handler.h"

void	command_simple_fill_redirect_in(
	t_command cmd, t_token **tks, int flags_mode[2], int *i)
{
	flags_mode[0] = O_RDONLY;
	flags_mode[1] = 0666;
	io_handlers_add_path(&cmd->io_handlers, \
		tks[++(*i)]->content, flags_mode, IO_IN);
}

void	command_simple_fill_redirect_out(
	t_command cmd, t_token **tks, int flags_mode[2], int *i)
{
	flags_mode[0] = O_WRONLY | O_CREAT;
	if (tks[*i]->type == OP_REDIRECT_OUT_TRUNC)
		flags_mode[0] |= O_TRUNC;
	else
		flags_mode[0] |= O_APPEND;
	flags_mode[1] = 0666;
	*i += 1;
	io_handlers_add_path(&cmd->io_handlers, \
		tks[*i]->content, flags_mode, IO_OUT);
}

void	command_simple_fill(
	t_command cmd, t_token **tokens, int endtoken_idx)
{
	int	i;
	int	j;
	int	flags_mode[2];

	i = -1;
	j = 0;
	while (++i < endtoken_idx)
	{
		if (tokens[i]->type == WORD)
			cmd->simple->cmd_argv[j++] = ft_strdup(tokens[i]->content);
		else if (tokens[i]->type == OP_REDIRECT_OUT_TRUNC
			|| tokens[i]->type == OP_REDIRECT_OUT_APPND)
		{
			command_simple_fill_redirect_out(cmd, tokens, flags_mode, &i);
			continue ;
		}
		else if (tokens[i]->type == OP_REDIRECT_IN)
		{
			command_simple_fill_redirect_in(cmd, tokens, flags_mode, &i);
			continue ;
		}
		else if (tokens[i]->type == OP_REDIRECT_IN_HEREDOC)
			io_handlers_add_heredoc(&cmd->io_handlers, tokens[++i]->content);
	}
}
