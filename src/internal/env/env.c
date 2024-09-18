/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:10:37 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/18 00:10:13 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "internal/default.h"
#include "internal/env/env_internal/env_internal.h"

void	env_initializer(void)
{
	extern char	**environ;
	int			i;

	i = DEFAULT_INIT;
	env_var_add_back(&env()->env_var, env_var_new(NULL, NULL));
	env_set_vars_size(ADD);
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
}

void	env_destroy(void)
{
	env_vars_destroy(env()->env_var);
}
