/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:38:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/25 22:21:12 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "internal/envp.h"
#include <sys/wait.h>
#include "internal/runner/runner.h"
#include "minishell.h"

int	runner(t_command cmd)
{
	pid_t	pid;
	int		status;

	cmd->simple->cmd_path = (
			envp_find_bin_by_name(cmd->simple->cmd_argv[0], __environ));
	pid = fork();
	if (pid == 0)
	{
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
