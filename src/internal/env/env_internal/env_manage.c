/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 02:25:56 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/26 07:45:09 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include "ft_string.h"
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/env/env_internal/env_internal.h"

static bool	check_key(char *key, t_env_var *env_var);

void	env_set_vars_size(t_operations type)
{
	if (type == ADD)
		env()->env_size++;
	else if (type == SUB)
		env()->env_size--;
}

t_env_var	*env_get_key(char *key, t_env_var *env_var)
{
	while (env_var)
	{
		if (env_var->key && check_key(key, env_var))
			return (env_var);
		env_var = env_var->next;
	}
	return (NULL);
}

void	env_update_default_values(void)
{
	t_env_var	*env_var;
	char		*value_update;

	env_var = env_get_key("SHLVL", env()->env_var);
	if (!env_var)
	{
		env_var_add_back(&env()->env_var, \
			env_var_new(ft_strdup("SHLVL"), ft_strdup("1")));
		env_set_vars_size(ADD);
	}
	else
	{
		value_update = ft_itoa(ft_atoi(env_var->value) + CHAR_BYTE);
		env_set_value("SHLVL", value_update, true);
		free(value_update);
	}
}

static bool	check_key(char *key, t_env_var *env_var)
{
	size_t	key_len;

	key_len = ft_strlen(env_var->key);
	if (ft_strlen(key) - CHAR_BYTE == key_len)
	{
		if (!ft_strncmp(env_var->key, key, ft_strlen(key) - NULL_BYTE))
			return (true);
	}
	if (ft_strlen(key) == key_len)
	{
		if (!ft_strncmp(env_var->key, key, ft_strlen(key) + NULL_BYTE))
			return (true);
	}
	else if (ft_strlen(key) + CHAR_BYTE == key_len)
	{
		if (env_var->key[key_len - CHAR_BYTE] == '=' \
			&& !ft_strncmp(env_var->key, key, key_len - CHAR_BYTE))
			return (true);
	}
	return (false);
}
