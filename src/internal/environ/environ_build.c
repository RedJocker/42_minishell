/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 01:57:31 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/14 03:54:00 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memlib.h"
#include "internal/default.h"
#include "internal/environ/environ_internal.h"

t_environ	*environ(void)
{
	static t_environ	environ;

	return (&environ);
}

t_environ_vars	*environ_var_new(char *key, char *value)
{
	t_environ_vars	*environ_var_new;

	environ_var_new = ft_calloc(CHAR_BYTE, sizeof(t_environ_vars));
	if (!environ_var_new)
		return (NULL);
	environ_var_new->key = key;
	environ_var_new->value = value;
	return (environ_var_new);
}

t_environ_vars	*environ_var_last(t_environ_vars *environ_var)
{
	if (!environ_var)
		return (NULL);
	while (environ_var->next)
		environ_var = environ_var->next;
	return (environ_var);
}

void	environ_var_add_back(t_environ_vars **environ_var, \
			t_environ_vars *environ_var_new)
{
	t_environ_vars	*environ_var_temp;

	if (!*environ_var)
		*environ_var = environ_var_new;
	else
	{
		environ_var_temp = environ_var_last(*environ_var);
		environ_var_temp->next = environ_var_new;
		environ_var_new->prev = environ_var_temp;
	}
}
