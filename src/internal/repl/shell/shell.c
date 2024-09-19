/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 06:34:42 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 09:35:52 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <stdbool.h>
#include "internal/repl/shell/shell_internal/shell_internal.h"
#include "internal/repl/shell/shell.h"

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
	shell()->redisplay_status = true;
	shell()->input = readline("RedWillShell$ ");
	shell()->redisplay_status = false;
}

void	shell_command(void)
{
	shell_build_token();
	shell_build_command();
	shell_build_runner();
}
