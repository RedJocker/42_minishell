/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 00:46:01 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/27 05:29:32 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "collection/ft_arraylist.h"
#include "ft_ctype.h"
#include "ft_string.h"
#include "ft_stdio.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/env/env_internal/env_internal.h"
#include "internal/env/envp.h"
#include "internal/repl/shell/command/command.h"

static sig_atomic_t	export_args(char *cmd, sig_atomic_t status);
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
			if (export_args(cmd->simple->cmd_argv[i], status) == EXIT_FAILURE)
				status = EXIT_FAILURE;
	return (status);
}

static sig_atomic_t	export_args(char *cmd, sig_atomic_t status)
{
	char	*key;
	char	*error_msg;

	error_msg = NULL;
	key = env_parse(cmd, KEY);
	if (*cmd != '=' && !ft_isdigit(*key) \
		&& !ft_strnstr(key, "-", ft_strlen(key)))
		export_valid_arg(cmd);
	else
	{
		status = EXIT_FAILURE;
		ft_asprintf(&error_msg, \
			"minishell: export: '%s': not a valid identifier\n", cmd);
		write(STDOUT_FILENO, error_msg, ft_strlen(error_msg));
		free(error_msg);
	}
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
	int			envp_i;
	t_arraylist	lst_new_envp;
	char		*env_var;

	lst_new_envp = NULL;
	envp_i = DEFAULT_INIT;
	while (envp && *envp)
	{
		if (!ft_strnstr(*envp, "$", ft_strlen(*envp)))
			ft_asprintf(&env_var, "declare -x %s\n", *envp++);
		else
		{
			lst_new_envp = ft_arraylist_new((t_consumer) free);
			while (envp[++envp_i])
			{
				if (*envp[envp_i] == '$')
					lst_new_envp = ft_arraylist_add(lst_new_envp, "\\");
				lst_new_envp = ft_arraylist_add(lst_new_envp, envp[envp_i]);
			}
			ft_asprintf(&env_var, "declare -x %s\n", ft_lststr_to_arrstr(lst_new_envp));
			free(lst_new_envp);
		}
		write(STDOUT_FILENO, env_var, ft_strlen(env_var));
		free(env_var);
	}
}
