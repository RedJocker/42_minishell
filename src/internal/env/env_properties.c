/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_properties.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 03:15:18 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/15 04:59:27 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "ft_util.h"
#include "ft_string.h"
#include "internal/default.h"
#include "internal/env/env_internal/env_internal.h"

char	*env_get_bin(char *bin)
{
	char	*bin_path;
	char	**env_path;

	if (access(bin, X_OK | F_OK | R_OK) == DEFAULT)
		return (ft_strdup(bin));
	if (*bin == '.' || *bin == '/')
		return (ft_strdup(bin));
	env_path = get_env_path();
	bin_path = build_bin_with_path(bin, env_path);
	if (env_path)
		ft_strarr_free(env_path);
	return (bin_path);
}

char	*env_get_value(char *key)
{
	t_env_var	*env_var;

	if (!key || env()->env_size == DEFAULT_BEGIN)
		return (NULL);
	env_var = env_get_key(key, env()->env_var);
	if (env_var)
	{
		if (env_var->value)
			return (ft_strdup(env_var->value));
		else
			return (NULL);
	}
	else
		return (NULL);
}

int	env_get_key_len(char *env_var)
{
	int	i;

	i = DEFAULT;
	while (env_var[i] && env_var[i] != '=')
		i++;
	if (env_var[i] == '=')
		i++;
	return (i);
}

void	env_set_value(char *key, char *value)
{
	size_t		key_len;
	t_env_var	*env_var;

	if (key)
	{
		env_var = env_get_key(key, env()->env_var);
		if (!env_var)
		{
			env_var_add_back(&env()->env_var, \
				env_var_new(ft_strdup(key), ft_strdup(value)));
			env_set_vars_size(ADD);
		}
		else
		{
			free(env_var->value);
			env_var->value = ft_strdup(value);
			key_len = ft_strlen(env_var->key);
			if (env_var->key[key_len - CHAR_BYTE] != '=')
			{
				free(env_var->key);
				env_var->key = ft_strdup(key);
			}
		}
	}
}
