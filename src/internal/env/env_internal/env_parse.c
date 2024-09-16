/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 03:41:36 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/16 04:32:48 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "internal/default.h"
#include "internal/env/env_internal/env_internal.h"

static int	get_key_len(char *env_var);

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
