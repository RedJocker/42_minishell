/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:10:37 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/13 04:45:14 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "ft_string.h"
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
	int		i;
	char	**environ_new;

	i = DEFAULT;
	// environ_new = NULL;
	// if (!__environ)
	// 	environ_status(NULL, SET);
	// else
	// {
		while (__environ[i])
			i++;
		environ_new = malloc(sizeof(char *) * (i + NULL_BYTE));
		i = DEFAULT_INIT;
		while (__environ[i++])
		{
			environ_new[i] = ft_strdup(__environ[i]);
		}
		environ_new[i] = NULL;
		environ_status(__environ, SET);
	// }
}
