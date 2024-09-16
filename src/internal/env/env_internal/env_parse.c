/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 03:41:36 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/16 05:52:22 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "internal/default.h"
#include "internal/env/env_internal/env_internal.h"
#include <stdio.h>
#include <stdlib.h>

static t_env_var	*get_key(char *key, t_env_var *env_var, size_t env_key_len);
static int			get_key_len(char *env_var);

char	*env_parse(char *env_var, t_type_env_var type)
{
	int		key_len;

	key_len = DEFAULT;
	if (type == KEY)
	{
		key_len = get_key_len(env_var);
		return (ft_substr(env_var, DEFAULT, key_len));
	}
	else if (type == VALUE)
	{
		if (env_var[key_len])
			return (ft_substr(env_var, key_len, ft_strlen(env_var) - key_len));
		else
			return (ft_strdup(""));
	}
	return (NULL);
}

t_env_var	*env_parse_key(char	*key)
{
	size_t		env_key_len;

	env_key_len = DEFAULT;
	if (!key || env()->env_size == DEFAULT_BEGIN)
		return (NULL);
	return (get_key(key, env()->env_var, env_key_len));
}

static t_env_var	*get_key(char *key, t_env_var *env_var, size_t env_key_len)
{
	while (env_var)
	{
		if (env_var->key)
		{
			env_key_len = ft_strlen(env_var->key);
			if (env_var->key[env_key_len - CHAR_BYTE] == '=')
			{
				if (ft_strlen(key) + CHAR_BYTE == env_key_len \
					&& !ft_strncmp(env_var->key, key, ft_strlen(key)))
					break ;
				env_var = env_var->next;
			}
			else if (!env_var->key[env_key_len - CHAR_BYTE])
			{
				if (ft_strlen(key) == env_key_len \
					&& !ft_strncmp(env_var->key, key, ft_strlen(key)))
					break ;
				env_var = env_var->next;
			}
		}
		else
			env_var = env_var->next;
	}
	return (env_var);
}

static int	get_key_len(char *env_var)
{
	int	i;

	i = DEFAULT;
	while (env_var[i] && env_var[i] != '=')
		i++;
	if (env_var[i] == '=')
		i++;
	return (i);
}
