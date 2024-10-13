/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 00:46:01 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/13 02:01:40 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_string.h"
#include "ft_stdio.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/env/envp.h"
#include "internal/repl/shell/command/command.h"

static void	export_without_args(void);
static void	export_sort_vars(char **envp);
static void	export_print_vars(char **envp);

int	runner_cmd_builtin_export(t_command cmd)
{
	if (DEFAULT_BEGIN == cmd->simple->cmd_argc)
		export_without_args();
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
