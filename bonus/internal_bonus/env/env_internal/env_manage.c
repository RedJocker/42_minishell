/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 02:25:56 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:11:18 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/env/env_internal/env_internal.h"

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
