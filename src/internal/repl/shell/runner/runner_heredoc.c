/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 09:43:30 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/05 00:58:54 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/repl/shell/command/command.h"
#include "runner.h"
#include "internal/repl/shell/command/io_handler.h"
#include "ft_assert.h"

void	runner_heredoc_prompt(t_command cmd)
{
	//ft_printf("runner heredoc %s\n", cmd->debug_id);
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
	{
		runner_heredoc_prompt(cmd->paren->cmd);
	}
	else if (cmd->type == CMD_INVALID)
		;
	else
		ft_assert(0, "runner_heredoc unexpected cmd type");
}

void	runner_heredoc_to_fd(t_command cmd)
{
	//ft_printf("runner heredoc %s\n", cmd->debug_id);
	if (cmd->type != CMD_SIMPLE)
		return ;
	io_handlers_heredoc_to_fd(cmd->io_handlers);
}
