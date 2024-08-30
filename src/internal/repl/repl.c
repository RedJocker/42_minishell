/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/08/30 02:58:48 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "ft_string.h"
#include "ft_util.h"
#include "ft_ctype.h"
#include "internal/default.h"
#include "internal/parse/parse.h"
#include "internal/runner/runner.h"
#include "internal/token/token.h"
#include "internal/command/command.h"

static int hystory_should_add_input(char *input)
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

int	repl(void)
{
	char		**str_tokens;
	t_token		**tokens;
	int			status;
	char		*input;
	int			tokens_len;
	t_command	command;

	status = 0;
	while (WAIT)
	{
		input = readline("RedWillShell$ ");
		if (hystory_should_add_input(input))
			add_history(input);
		else if (input)
			continue ;
		str_tokens = parse(input);
		//ft_strarr_printfd(str_tokens, 1);
		free(input);
		tokens = tokens_classify(str_tokens, &tokens_len);
		ft_strarr_free(str_tokens);
		//tokens_print(tokens);
		command = command_build(tokens, tokens_len);
		tokens_destroy(tokens);
		status = runner(command);
		command_destroy(command);
	}
	return (status);
}
