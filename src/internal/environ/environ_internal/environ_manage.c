/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_manage.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 02:25:56 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/15 03:31:24 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/default.h"
#include "internal/environ/environ_internal/environ_internal.h"

void	environ_set_vars_size(t_operations type)
{
	if (type == ADD)
		environ()->environ_size++;
	else if (type == SUB)
		environ()->environ_size--;
}
