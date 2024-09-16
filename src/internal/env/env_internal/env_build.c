/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 01:57:31 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/16 04:38:28 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memlib.h"
#include "internal/default.h"
#include "internal/env/env_internal/env_internal.h"

t_env	*env(void)
{
	static t_env	env;

	return (&env);
}

t_env_var	*env_var_new(char *key, char *value)
{
	t_env_var	*env_var_new;

	env_var_new = ft_calloc(CHAR_BYTE, sizeof(t_env_var));
	if (!env_var_new)
		return (NULL);
	env_var_new->key = key;
	env_var_new->value = value;
	env_var_new->next = NULL;
	env_var_new->prev = NULL;
	return (env_var_new);
}

t_env_var	*env_var_last(t_env_var *env_var)
{
	if (!env_var)
		return (NULL);
	while (env_var->next)
		env_var = env_var->next;
	return (env_var);
}

void	env_var_add_back(t_env_var **env_var, t_env_var *env_var_new)
{
	t_env_var	*env_var_temp;

	if (!*env_var)
		*env_var = env_var_new;
	else
	{
		env_var_temp = env_var_last(*env_var);
		env_var_temp->next = env_var_new;
		env_var_new->prev = env_var_temp;
	}
}
