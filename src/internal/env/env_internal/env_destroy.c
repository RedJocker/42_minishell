/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_destroy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 02:33:40 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/18 04:51:55 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "internal/env/env_internal/env_internal.h"

void	env_vars_destroy(t_env_var *env_var)
{
	t_env_var	*env_var_temp;

	while (env_var)
	{
		env_var_temp = env_var;
		if (env_var->key)
			free(env_var->key);
		if (env_var->value)
			free(env_var->value);
		env_var = env_var->next;
		free(env_var_temp);
	}
}

void	env_var_destroy(t_env_var *env_var)
{
	t_env_var	*env_var_temp;

	env_var_temp = env_var;
	if (env_var->prev)
		env_var->prev->next = env_var_temp->next;
	if (env_var->next)
		env_var->next->prev = env_var_temp->prev;
	if (env_var_temp->key)
		free(env_var_temp->key);
	if (env_var_temp->value)
		free(env_var_temp->value);
	free(env_var_temp);
	env_set_vars_size(SUB);
}
