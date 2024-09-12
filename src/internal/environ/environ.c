/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:10:37 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/12 02:33:05 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "internal/default.h"

char	**environ_status(char **value, int type)
{
	static char	**environ;

	if (type == SET)
		environ = value;
	return (environ);
}

void	environ_initializer(void)
{
	environ_status(__environ, SET);
}
