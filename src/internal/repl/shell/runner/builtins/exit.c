/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 05:02:21 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/26 07:12:58 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal/default.h"
#include "internal/ft_extension.h"
#include "internal/repl/shell/command/command.h"

static void	print_exit_error(char *cmd);

sig_atomic_t	runner_cmd_builtin_exit(t_command cmd)
{
	sig_atomic_t	status;
	long long		nbr_status;
	char			*nbr_endptr;

	status = EXIT_OK;
	write(STDOUT_FILENO, "exit\n", ft_strlen("exit\n"));
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		return (status);
	else if (cmd->simple->cmd_argc > MAX_ARGS)
	{
		print_exit_error(NULL);
		return (EXIT_FAILURE);
	}
	nbr_status = ft_strtoll(cmd->simple->cmd_argv[DEFAULT_BEGIN], &nbr_endptr);
	if (*nbr_endptr)
	{
		print_exit_error(cmd->simple->cmd_argv[DEFAULT_BEGIN]);
		return (EXIT_SYNTAX_ERROR);
	}
	return ((unsigned char)(nbr_status % MAX_STATUS_CODE_SIZE));
}

static void	print_exit_error(char *cmd)
{
	char	*error_msg;

	if (!cmd)
		ft_asprintf(&error_msg, \
			"minishell: exit: too many arguments\n");
	else
		ft_asprintf(&error_msg, \
			"minishell: exit: %s: numeric argument required\n", cmd);
	write(STDOUT_FILENO, error_msg, ft_strlen(error_msg));
	free(error_msg);
}
