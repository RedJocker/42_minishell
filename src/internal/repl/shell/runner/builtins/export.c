/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 00:46:01 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/16 06:45:51 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include "ft_string.h"
#include "ft_stdio.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/env/env_internal/env_internal.h"
#include "internal/env/envp.h"
#include "internal/repl/shell/command/command.h"

static sig_atomic_t	export_args(char *cmd, sig_atomic_t status);
static void			export_without_args(void);
static void			export_sort_vars(char **envp);
static void			export_print_vars(char **envp);

sig_atomic_t	runner_cmd_builtin_export(t_command cmd)
{
	int				i;
	sig_atomic_t	status;

	i = DEFAULT;
	status = EXIT_OK;
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		export_without_args();
	else
	{
		while (cmd->simple->cmd_argv[++i])
			if (export_args(cmd->simple->cmd_argv[i], status) == EXIT_FAILURE)
				status = EXIT_FAILURE;
	}
	return (status);
}

static sig_atomic_t	export_args(char *cmd, sig_atomic_t status)
{
	char	*key;
	char	*value;
	char	*error_msg;

	key = NULL;
	value = NULL;
	error_msg = NULL;
	if (*cmd != '=')
	{
		key = env_parse(cmd, KEY);
		value = env_parse(cmd, VALUE);
		env_set_value(key, value);
		free(key);
		free(value);
	}
	else
	{
		status = EXIT_FAILURE;
		ft_asprintf(&error_msg, \
			"bash: export: %s: not a valid identifier\n", cmd); // TODO: change the name of program to minishell instead of bash.
		write(STDOUT_FILENO, error_msg, ft_strlen(error_msg));
		free(error_msg);
	}
	return (status);
}

static void	export_without_args(void)
{
	char	**envp;

	envp = get_envp(ENVP_EXPORT);
	export_sort_vars(envp);
	export_print_vars(envp);
	if (envp)
		ft_strarr_free(envp);
}

static void	export_sort_vars(char **envp)
{
	char	*swap;
	int		i;
	int		next_i;

	i = DEFAULT_INIT;
	while (envp[++i])
	{
		next_i = i;
		while (envp[++next_i])
		{
			if (ft_strncmp(envp[i], envp[next_i], \
				ft_strlen(envp[next_i])) > DEFAULT)
			{
				swap = envp[i];
				envp[i] = envp[next_i];
				envp[next_i] = swap;
			}
		}
	}
}

static void	export_print_vars(char **envp)
{
	char	*env_var;

	while (envp && *envp)
	{
		ft_asprintf(&env_var, "declare -x %s\n", *envp++);
		write(STDOUT_FILENO, env_var, ft_strlen(env_var));
		free(env_var);
	}
}
