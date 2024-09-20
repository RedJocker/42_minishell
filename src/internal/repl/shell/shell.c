/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 06:34:42 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 21:23:09 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include "internal/repl/shell/shell_internal/shell_internal.h"
#include "internal/repl/shell/shell.h"
#include "internal/signal/signal.h"


bool	shell_get_redisplay_status(void)
{
	return (shell()->redisplay_status);
}

char	*shell_get_input(void)
{
	return (shell()->input);
}

void	shell_set_input(void)
{
	int	should_redisplay;

	should_redisplay = 1;
	signals_initializer(should_redisplay);
	shell()->input = readline("RedWillShell$ ");
	should_redisplay = 0;
	signals_initializer(should_redisplay);
}

void	shell_command(void)
{
	shell_build_token();
	shell_build_command();
	shell_build_runner();
}
