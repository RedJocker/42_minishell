/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 05:02:21 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/20 01:35:44 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/repl/shell/command/command.h"

sig_atomic_t	runner_cmd_builtin_exit(t_command cmd)
{
	sig_atomic_t	status;
	long long		nbr_status;
	char			*nbr_endptr;

	status = EXIT_OK;
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		return (status);
	nbr_status = ft_strtoll(cmd->simple->cmd_argv[DEFAULT_BEGIN], &nbr_endptr);
	if (*nbr_endptr)
		return (EXIT_SYNTAX_ERROR);
	else if (cmd->simple->cmd_argc > MAX_ARGS)
		return (EXIT_FAILURE);
	return ((unsigned char)(nbr_status % MAX_STATUS_CODE_SIZE));
}

void	print_exit_error(char *cmd)
{
	char	*error_msg;

	if (!cmd)
		ft_asprintf(&error_msg, \
			"minishell: exit: too many arguments\n");
	else
		ft_asprintf(&error_msg, \
			"minishell: exit: %s: numeric argument required\n", cmd);
	write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	free(error_msg);
}
