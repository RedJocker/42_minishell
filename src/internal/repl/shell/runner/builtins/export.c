/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 00:46:01 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/29 04:21:55 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "ft_ctype.h"
#include "ft_string.h"
#include "ft_stdio.h"
#include "ft_util.h"
#include "stringbuilder.h"
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/env/env_internal/env_internal.h"
#include "internal/env/envp.h"
#include "internal/repl/shell/command/command.h"

static sig_atomic_t	export_args(char *cmd, char *key, sig_atomic_t status);
static void			export_valid_arg(char *cmd);
static void			export_sort_vars(char **envp);
static void			export_print_vars(char **envp);

sig_atomic_t	runner_cmd_builtin_export(t_command cmd)
{
	int				i;
	sig_atomic_t	status;
	char			**envp;

	i = DEFAULT;
	status = EXIT_OK;
	envp = NULL;
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
	{
		envp = get_envp(ENVP_EXPORT);
		export_sort_vars(envp);
		export_print_vars(envp);
		if (envp)
			ft_strarr_free(envp);
	}
	else
		while (cmd->simple->cmd_argv[++i])
			if (export_args(cmd->simple->cmd_argv[i], \
				env_parse(cmd->simple->cmd_argv[i], KEY), \
				status) == EXIT_FAILURE)
				status = EXIT_FAILURE;
	return (status);
}

static sig_atomic_t	export_args(char *cmd, char *key, sig_atomic_t status)
{
	char	*error_msg;

	error_msg = NULL;
	if (*cmd == '-' && !ft_strnstr(key, "=", ft_strlen(key)))
		status = EXIT_OK;
	else if (*cmd == '-' && ft_strnstr(key, "=", ft_strlen(key)))
		ft_asprintf(&error_msg, \
			"minishell: export: `-=': not a valid identifier\n");
	else if (*cmd != '=' && !ft_isdigit(*key) \
		&& !ft_strnstr(key, "-", ft_strlen(key)))
		export_valid_arg(cmd);
	else
		ft_asprintf(&error_msg, \
			"minishell: export: `%s': not a valid identifier\n", cmd);
	if (error_msg)
	{
		status = EXIT_FAILURE;
		write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
		free(error_msg);
	}
	free(key);
	return (status);
}

static void	export_valid_arg(char *cmd)
{
	bool	equal;
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	equal = false;
	key = env_parse(cmd, KEY);
	value = env_parse(cmd, VALUE);
	if (key[ft_strlen(key) - CHAR_BYTE] == '=')
		equal = true;
	env_set_value(key, value, equal);
	free(key);
	free(value);
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
	char			*temp_envp;
	char			*env_var;
	int				envp_i;
	t_stringbuilder	builder_new_envp;

	while (envp && *envp)
	{
		envp_i = DEFAULT_INIT;
		builder_new_envp = stringbuilder_new();
		while (envp[DEFAULT][++envp_i])
		{	
			if (envp[DEFAULT][envp_i] == '$')
				builder_new_envp = stringbuilder_addchar(\
					builder_new_envp, '\\');
			builder_new_envp = stringbuilder_addchar(\
					builder_new_envp, envp[DEFAULT][envp_i]);
		}
		temp_envp = stringbuilder_build(builder_new_envp);
		ft_asprintf(&env_var, "declare -x %s\n", temp_envp);
		free(temp_envp);
		envp++;
		write(STDOUT_FILENO, env_var, ft_strlen(env_var));
		free(env_var);
	}
}
