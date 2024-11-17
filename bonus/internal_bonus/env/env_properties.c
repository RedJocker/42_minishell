/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_properties.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 03:15:18 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:10:48 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_util.h"
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/env/env_internal/env_internal.h"

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

void	env_set_value(char *key, char *value, bool equal)
{
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
			if (equal)
			{
				free(env_var->value);
				env_var->value = ft_strdup(value);
			}
			if (env_var->key[ft_strlen(env_var->key) - CHAR_BYTE] != '=')
			{
				free(env_var->key);
				env_var->key = ft_strdup(key);
			}
		}
	}
}

char	*env_parse(char *env_var, t_type_env_var type)
{
	int	key_len;

	key_len = DEFAULT;
	while (env_var[key_len] && env_var[key_len] != '=')
		key_len++;
	if (env_var[key_len] == '=')
		key_len++;
	if (type == KEY)
		return (ft_substr(env_var, DEFAULT, key_len));
	else if (type == VALUE)
	{
		if (env_var[key_len])
			return (ft_substr(env_var, key_len, ft_strlen(env_var) - key_len));
		else
			return (ft_strdup(""));
	}
	return (NULL);
}
