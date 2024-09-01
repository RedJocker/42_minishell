/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/01 16:54:58 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "ft_util.h"
#include "ft_ctype.h"
#include "internal/default.h"
#include "internal/parse/parse.h"
#include "internal/runner/runner.h"
#include "internal/signal/signal.h"
#include "internal/token/token.h"
#include "internal/command/command.h"
#include "internal/signal/terminal.h"
#include "repl.h"

static int	hystory_should_add_input(char *input)
{
	if (!input)
		return (0);
	while (*input)
	{
		if (!ft_isspace(*input))
			return (1);
		input++;
	}
	return (0);
}

int	repl_loop(t_repl *mini)
{
	mini->input = readline("RedWillShell$ ");
	if (hystory_should_add_input(mini->input))
		add_history(mini->input);
	else if (mini->input)
	{
		free(mini->input);
		return (0);
	}
	mini->str_tokens = parse(mini->input);
	//ft_strarr_printfd(mini->str_tokens, 1);
	free(mini->input);
	mini->tokens = tokens_classify(mini->str_tokens, &mini->tokens_len);
	ft_strarr_free(mini->str_tokens);
	//tokens_print(mini->tokens);
	mini->command = command_build(mini->tokens, mini->tokens_len);
	tokens_destroy(mini->tokens);
	mini->status = runner(mini->command);
	signal_status(mini->status, SET);
	command_destroy(mini->command);
	terminal_properties(true);
	return (0);
}

int	repl(void)
{
	t_repl	mini;
	int		should_close;

	mini.status = 0;
	terminal_properties(false);
	should_close = 0;
	while (!should_close)
		should_close = repl_loop(&mini);
	return (signal_status(DEFAULT, GET));
}
