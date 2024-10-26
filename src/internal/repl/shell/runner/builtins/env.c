/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:50:37 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/26 07:13:14 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "ft_string.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/env/envp.h"
#include "internal/repl/shell/command/command.h"

static void	env_without_args(void);
static void	env_print_vars(char **envp);

sig_atomic_t	runner_cmd_builtin_env(t_command cmd)
{
	sig_atomic_t	status;
	char			*error_msg;

	status = EXIT_OK;
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		env_without_args();
	else
	{
		status = EXIT_FAILURE;
		ft_asprintf(&error_msg, \
			"minishell: env: env doesn't accept arguments or flags\n", cmd);
		write(STDOUT_FILENO, error_msg, ft_strlen(error_msg));
		free(error_msg);
	}
	return (status);
}

static void	env_without_args(void)
{
	char	**envp;

	envp = get_envp(ENVP_DEFAULT);
	env_print_vars(envp);
	if (envp)
		ft_strarr_free(envp);
}

static void	env_print_vars(char **envp)
{
	char	*env_var;

	while (envp && *envp)
	{
		ft_asprintf(&env_var, "%s\n", *envp++);
		write(STDOUT_FILENO, env_var, ft_strlen(env_var));
		free(env_var);
	}
}
