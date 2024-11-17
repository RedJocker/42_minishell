/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:10:37 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:10:59 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "internal_bonus/default.h"
#include "internal_bonus/env/env.h"
#include "internal_bonus/env/env_internal/env_internal.h"

void	env_initializer(void)
{
	extern char	**environ;
	int			i;

	i = DEFAULT_INIT;
	if (environ)
	{
		while (environ[++i])
		{
			env_var_add_back(&env()->env_var, \
				env_var_new(env_parse(environ[i], KEY), \
				env_parse(environ[i], VALUE)));
			env_set_vars_size(ADD);
		}
	}
	env_update_default_values();
}

void	env_destroy(void)
{
	env_vars_destroy(env()->env_var);
}

void	env_destroy_single_var(char *key)
{
	t_env_var	*env_var;

	env_var = env_get_key(key, env()->env_var);
	if (env_var)
		env_var_destroy(env_var);
}
