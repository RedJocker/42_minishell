/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/08/24 01:31:35 by maurodri         ###   ########.fr       */
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

int	repl(void)
{
	char	**str_tokens;
	t_token	**tokens;
	int		status;
	char	*input;

	status = 0;
	while (1)
	{
		input = readline("RedWillShell$ ");
		if (!input)
			break ;
		str_tokens = parse(input);
		free(input);
		tokens = tokens_classify(str_tokens);
		tokens_print(tokens);
		tokens_destroy(tokens);
		status = runner(str_tokens);
		ft_strarr_free(str_tokens);
	}
	return (status);
}
