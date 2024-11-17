/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 09:43:30 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:03:04 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_assert.h"
#include "internal_bonus/repl/shell/command/command.h"
#include "internal_bonus/repl/shell/command/io_handler.h"

void	runner_heredoc_prompt(t_command cmd)
{
	if (cmd->type == CMD_SIMPLE)
		io_handlers_heredoc_prompt(cmd->io_handlers);
	else if (cmd->type == CMD_PIPE)
	{
		runner_heredoc_prompt(cmd->pipe->cmd_before);
		runner_heredoc_prompt(cmd->pipe->cmd_after);
	}
	else if (cmd->type == CMD_AND)
	{
		runner_heredoc_prompt(cmd->and->cmd_before);
		runner_heredoc_prompt(cmd->and->cmd_after);
	}
	else if (cmd->type == CMD_OR)
	{
		runner_heredoc_prompt(cmd->or->cmd_before);
		runner_heredoc_prompt(cmd->or->cmd_after);
	}
	else if (cmd->type == CMD_PAREN)
		runner_heredoc_prompt(cmd->paren->cmd);
	else if (cmd->type == CMD_INVALID)
		;
	else
		ft_assert(0, "runner_heredoc unexpected cmd type");
}

void	runner_heredoc_to_fd(t_command cmd)
{
	if (cmd->type != CMD_SIMPLE)
		return ;
	io_handlers_heredoc_to_fd(cmd->io_handlers);
}
