/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 01:25:54 by dande-je          #+#    #+#             */
/*   Updated: 2024/08/16 21:20:45 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "internal/parse/parse.h"

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	parse();
	exit(EXIT_SUCCESS);
}
