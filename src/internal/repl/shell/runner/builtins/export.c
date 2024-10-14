/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 00:46:01 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/14 04:04:38 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_string.h"
#include "ft_stdio.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/env/env_internal/env_internal.h"
#include "internal/env/env.h"
#include "internal/env/envp.h"
#include "internal/repl/shell/command/command.h"

static void	export_without_args(void);
static void	export_sort_vars(char **envp);
static void	export_print_vars(char **envp);

int	runner_cmd_builtin_export(t_command cmd)
{
	int			i;
	int			key_len;
	char		*key;
	// t_env_var	*env_var;

	i = DEFAULT;
	key_len = DEFAULT;
	key = NULL;
	// env_var = NULL;
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		export_without_args();
	else
	{
		while (cmd->simple->cmd_argv[++i])
		{
			key_len = env_get_key_len(cmd->simple->cmd_argv[i]);
			key = ft_substr(cmd->simple->cmd_argv[i], DEFAULT, key_len);
			env_set_value(key, env_parse(cmd->simple->cmd_argv[i], VALUE));
			// env_var = env_get_key(key, env()->env_var);
			// if (!env_var)
			// {
			// }
			// else
			// {
				// free(env_var->key);
				// free(env_var->value);
				// env_var->key = ft_strdup(key);
				// env_var->value = env_parse(cmd->simple->cmd_argv[i], VALUE);
			// }
			free(key);
			// free(env_var);
		}
	}
	return (EXIT_OK);
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
