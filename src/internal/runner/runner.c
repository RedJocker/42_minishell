/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:38:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/23 03:35:59 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "internal/envp.h"
#include <sys/wait.h>
#include "internal/runner/runner.h"

int	runner(char **arr)
{
	char	*cmd;
	pid_t	pid;
	int		status;

	cmd = envp_find_bin_by_name(arr[0], __environ);
	pid = fork();
	if (pid == 0)
	{
		execve(cmd, arr, __environ);
		status = 1;
	}
	else
	{
		waitpid(pid, 0, 0);
		status = 0;
	}
	free(cmd);
	return (status);
}
