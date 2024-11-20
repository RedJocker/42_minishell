/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 05:02:21 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/20 20:46:13 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ctype.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal/repl/shell/runner/builtins/builtins.h"

static int	is_blank(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

static int	start_with_numeric(char *str)
{
	while (*str && (*str == '+' || *str == '-' || ft_isspace(*str)))
		str++;
	return (ft_isdigit(*str));
}

sig_atomic_t	runner_cmd_builtin_exit(t_command cmd)
{
	sig_atomic_t	status;
	long long		nbr_status;
	char			*nbr_endptr;

	status = EXIT_OK;
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		return (status);
	if (!start_with_numeric(cmd->simple->cmd_argv[DEFAULT_BEGIN]))
		return (EXIT_SYNTAX_ERROR);
	nbr_status = ft_strtoll(cmd->simple->cmd_argv[DEFAULT_BEGIN], &nbr_endptr);
	if (!is_blank(nbr_endptr))
		return (EXIT_SYNTAX_ERROR);
	if (cmd->simple->cmd_argc > MAX_ARGS)
		return (-1);
	return ((unsigned char)(nbr_status % MAX_STATUS_CODE_SIZE));
}

void	print_exit_error(char *cmd)
{
	char	*error_msg;

	if (cmd)
		ft_asprintf(&error_msg, \
			"minishell: exit: %s: numeric argument required\n", cmd);
	else
		ft_asprintf(&error_msg, \
			"minishell: exit: too many arguments\n");
	write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	free(error_msg);
}

void	print_builtin_exit(
	t_builtin_id builtin,
	t_runner_data *run_data,
	sig_atomic_t status)
{
	if (builtin == BUILTIN_EXIT)
	{
		if (run_data->is_main)
			ft_putendl("exit");
		if (status == EXIT_SYNTAX_ERROR)
			print_exit_error(run_data->cmd->simple->cmd_argv[DEFAULT_BEGIN]);
		else if (status == -1)
			print_exit_error(NULL);
	}
}
