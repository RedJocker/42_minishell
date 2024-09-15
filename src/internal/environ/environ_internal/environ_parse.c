/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 03:41:36 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/15 05:27:50 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "stdio.h"
void	environ_parse(char *environ_var)
{
	char	**environ_var_split;

	printf("sem split |%s|\n", environ_var);
	environ_var_split = ft_split(environ_var, '=');
	printf("key = |%s| - valur = |%s|\n", environ_var_split[0], environ_var_split[1]);
}
