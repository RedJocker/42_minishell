/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/08/23 03:36:04 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "ft_util.h"
#include "internal/parse/parse.h"
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "internal/runner/runner.h"

int	repl(void)
{
	char	**arr;
	int		status;
	char	*input;

	status = 0;
	while (1)
	{
		input = readline("RedWillShell$ ");
		if (!input)
			break ;
		arr = parse(input);
		free(input);
		status = runner(arr);
		ft_strarr_free(arr);
	}
	return (status);
}
