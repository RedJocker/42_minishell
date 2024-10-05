/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:12:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/03 00:06:05 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memlib.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal/default.h"
#include "internal/env/env_internal/env_internal.h"

char	**get_envp(void)
{
	t_env_var	*env_vars;
	char		*env_var;
	char		**envp;
	int			i;

	env_vars = env()->env_var;
	if (env_vars)
	{
		i = DEFAULT_INIT;
		envp = ft_calloc(env()->env_size + NULL_BYTE, sizeof(char *));
		while (env_vars)
		{
			ft_asprintf(&env_var, "%s%s", env_vars->key, env_vars->value);
			envp[++i] = env_var;
			env_vars = env_vars->next;
		}
		envp[++i] = NULL;
		return (envp);
	}
	else
		return (NULL);
}
