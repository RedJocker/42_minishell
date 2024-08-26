/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/08/25 22:19:21 by maurodri         ###   ########.fr       */
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
#include "internal/token/token.h"
#include "internal/command/command.h"

int	repl(void)
{
	char		**str_tokens;
	t_token		**tokens;
	int			status;
	char		*input;
	int			tokens_len;
	t_command	command;

	status = 0;
	while (1)
	{
		input = readline("RedWillShell$ ");
		if (!input)
			break ;
		str_tokens = parse(input);
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
