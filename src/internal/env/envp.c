/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:12:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/28 05:43:42 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "internal/env/env_internal/env_internal.h"

char	**get_envp(void)
{
	t_env_var	*env_vars;

	env_vars = env_parse_key("PATH");
	if (env_vars)
		return (ft_split(env_vars->value, ':'));
	else
		return (NULL);
}
