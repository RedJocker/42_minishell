/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:38:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/29 20:57:07 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "internal/envp.h"
#include "internal/command/command.h"
#include "internal/command/io_handler.h"
#include "runner.h"
#include "internal/ft_extensions.h"
#include <stdio.h>

int	runner(t_command cmd)
{
	pid_t	pid;
	int		status;

	cmd->simple->cmd_path = (
			envp_find_bin_by_name(cmd->simple->cmd_argv[0], __environ));
	pid = fork();
	if (pid == 0)
	{
		io_handler_redirect(&cmd->output, STDOUT);
		if (cmd->output.type == IO_ERROR)
		{
			//TODO
			ft_assert(0, "TODO0");
			return (1);
		}
		execve(cmd->simple->cmd_path, cmd->simple->cmd_argv, __environ);
		status = 1;
	}
	else
	{
		waitpid(pid, 0, 0);
		status = 0;
	}
	return (status);
}
