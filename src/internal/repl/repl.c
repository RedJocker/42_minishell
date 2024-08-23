/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/08/23 01:29:02 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "ft_util.h"
#include "internal/parse/parse.h"
#include "internal/envp.h"
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

void	repl(void)
{
	char **arr;
	pid_t	pid;

	while (1)
	{
		char *input = readline("RedWillShell$ ");
		if (!input)
			break ;

		arr = parse(input);
		char *cmd = envp_find_bin_by_name(arr[0], __environ);
		pid = fork();
		if (pid == 0)
		{
			execve(cmd, arr, __environ);
			free(input);
			free(cmd);
			ft_strarr_free(arr);
		}
		else
		{
			waitpid(pid, 0, 0);
			free(input);
			free(cmd);
			ft_strarr_free(arr);
		}
	}
}
