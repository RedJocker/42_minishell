/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 01:25:54 by dande-je          #+#    #+#             */
/*   Updated: 2024/08/16 23:16:51 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "ft_string.h"
#include "internal/parse/parse.h"
#include "get_next_line.h"
#include "ft_stdio.h"
#include "internal/envp.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "ft_util.h"

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	pid_t	pid;
	int 	len;
	
	while (1)
	{
		char *input = ft_chomp(get_next_line(0));
		if (!input)
			break ;
		char **arr = ft_split(input, ' ');
		char *cmd = envp_find_bin_by_name(arr[0], envp);
		pid = fork();
		if (pid == 0)
		{
			len = 0;
			execve(cmd, arr, envp);
			free(input);
			free(cmd);
			free(arr);
		}
		else
		{
			waitpid(pid, 0, 0);
			free(input);
			free(cmd);
			free(arr);
		}
	}
	exit(EXIT_SUCCESS);
}
