/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_destroy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 02:33:40 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/14 03:18:44 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "internal/environ/environ_internal.h"

void	environ_destroy(t_environ_vars *environ_var)
{
	t_environ_vars	*environ_vars_temp;

	while (environ_var)
	{
		environ_vars_temp = environ_var;
		environ_var = environ_var->next;
		free(environ_vars_temp);
	}
}
