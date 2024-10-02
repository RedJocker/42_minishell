/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:10:37 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/02 04:55:39 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "ft_util.h"
#include "ft_string.h"
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

char	*env_get_bin(char *bin)
{
	char	*bin_path;
	char	**env_path;

	if (access(bin, X_OK | F_OK | R_OK) == DEFAULT)
		return (ft_strdup(bin));
	if (*bin == '.' || *bin == '/' || *bin == '~')
		return (ft_strdup(bin));
	env_path = get_env_path();
	bin_path = build_bin_with_path(bin, env_path);
	if (env_path)
		ft_strarr_free(env_path);
	return (bin_path);
}

char	*env_get_value(char *key)
{
	size_t		env_key_len;

	env_key_len = DEFAULT;
	if (!key || env()->env_size == DEFAULT_BEGIN)
		return (NULL);
	return (env_get_key(key, env()->env_var, env_key_len)->value);
}

void	env_destroy(void)
{
	env_vars_destroy(env()->env_var);
}
