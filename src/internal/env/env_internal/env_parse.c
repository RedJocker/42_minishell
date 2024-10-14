/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 03:41:36 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/14 03:32:17 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/env/env_internal/env_internal.h"

char	*env_parse(char *env_var, t_type_env_var type)
{
	int		key_len;

	key_len = env_get_key_len(env_var);
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
