/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 06:34:42 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 08:36:01 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <stdbool.h>
#include "ft_util.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/parse/parse.h"
#include "internal/repl/shell/runner/runner.h"
#include "internal/repl/shell/token/token.h"
#include "internal/signal/signal.h"
#include "internal/repl/shell/shell.h"

t_shell	*shell(void)
{
	static t_shell	shell;

	return (&shell);
}

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

void	shell_build_token(void)
{
	shell()->str_tokens = parse(shell()->input);
	//ft_strarr_printfd(shell()->str_tokens, 1);
	free(shell()->input);
	shell()->tokens = tokens_classify(shell()->str_tokens, &shell()->tokens_len);
	ft_strarr_free(shell()->str_tokens);
	//tokens_print(shell()->tokens);
}

void	shell_build_command(void)
{
	shell()->command = command_build(shell()->tokens, shell()->tokens_len);
	tokens_destroy(shell()->tokens);
}

void	shell_run_command(void)
{
	shell()->status = runner(shell()->command, shell()->status);
	signal_status(shell()->status, SET);
	command_destroy(shell()->command);
}
