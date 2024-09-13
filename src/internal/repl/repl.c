/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/13 01:09:48 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ft_ctype.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/environ/environ.h"
#include "internal/repl/parse/parse.h"
#include "internal/repl/command/command.h"
#include "internal/repl/repl.h"
#include "internal/repl/runner/runner.h"
#include "internal/signal/signal.h"
#include "internal/signal/terminal.h"
#include "internal/token/token.h"

static bool	history_add_input(char *input);

void	repl_loop(void)
{
	mini()->redisplay_status = true;
	mini()->input = readline("RedWillShell$ ");
	mini()->redisplay_status = false;
	if (history_add_input(mini()->input))
		add_history(mini()->input);
	else if (mini()->input)
		return ;
	mini()->str_tokens = parse(mini()->input);
	//ft_strarr_printfd(mini()->str_tokens, 1);
	free(mini()->input);
	mini()->tokens = tokens_classify(mini()->str_tokens, &mini()->tokens_len);
	ft_strarr_free(mini()->str_tokens);
	//tokens_print(mini()->tokens);
	mini()->command = command_build(mini()->tokens, mini()->tokens_len);
	tokens_destroy(mini()->tokens);
	mini()->status = runner(mini()->command, mini()->status);
	signal_status(mini()->status, SET);
	command_destroy(mini()->command);
	terminal_properties(true);
}

int	repl(void)
{
	signals_initializer();
	environ_initializer();
	terminal_properties(false);
	while (WAIT)
		repl_loop();
	return (signal_status(DEFAULT, GET));
}

t_repl	*mini(void)
{
	static t_repl	mini;

	return (&mini);
}

static bool	history_add_input(char *input)
{
	if (!input)
		return (false);
	// TODO: why else if not work?
	// else if (!ft_isspace(*input))
	// 	return (true);
	while (*input)
	{
		if (!ft_isspace(*input))
			return (true);
		input++;
	}
	return (false);
}
