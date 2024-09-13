/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:06:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/13 00:55:46 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_assert.h"
#include "ft_string.h"
#include "internal/repl/parse/parse_internal.h"

char	*parse_operator_newline(char **input)
{
	*input = (*input) + 1;
	return (ft_strdup("\n"));
}

int	parse_is_operator(char maybe_operator)
{
	return (maybe_operator == '>'
		|| maybe_operator == '<'
		|| maybe_operator == '('
		|| maybe_operator == ')'
		|| maybe_operator == '&'
		|| maybe_operator == '{'
		|| maybe_operator == '|');
}

char	*parse_operator_redirect_out(char **input)
{
	if (ft_strncmp(input[0], ">>", 2) == 0)
	{
		*input = (*input) + 2;
		return (ft_strdup(">>"));
	}
	else if (ft_strncmp(input[0], ">", 1) == 0)
	{
		*input = (*input) + 1;
		return (ft_strdup(">"));
	}
	ft_assert(0, "unexpected input on parse_redirect_out");
	return (NULL);
}

char	*parse_operator_redirect_in(char **input)
{
	if (ft_strncmp(input[0], "<<", 2) == 0)
	{
		*input = (*input) + 2;
		return (ft_strdup("<<"));
	}
	else if (ft_strncmp(input[0], "<", 1) == 0)
	{
		*input = (*input) + 1;
		return (ft_strdup("<"));
	}
	ft_assert(0, "unexpected input on parse_redirect_in");
	return (NULL);
}

char	*parse_operator_paren_open(char **input)
{
	if (ft_strncmp(input[0], "(", 1) == 0)
	{
		*input = (*input) + 1;
		return (ft_strdup("("));
	}
	ft_assert(0, "unexpected input on parse_paren_open");
	return (NULL);
}

char	*parse_operator_paren_close(char **input)
{
	if (ft_strncmp(input[0], ")", 1) == 0)
	{
		*input = (*input) + 1;
		return (ft_strdup(")"));
	}
	ft_assert(0, "unexpected input on parse_paren_close");
	return (NULL);
}

char	*parse_operator_opand(char **input)
{
	if (ft_strncmp(input[0], "&&", 2) == 0)
	{
		*input = (*input) + 2;
		return (ft_strdup("&&"));
	}
	else if (ft_strncmp(input[0], "&", 1) == 0)
	{
		*input = (*input) + 1;
		return (ft_strdup("&"));
	}
	ft_assert(0, "unexpected input on parse_opand");
	return (NULL);
}

char	*parse_operator_pipe_or_opor(char **input)
{
	if (ft_strncmp(input[0], "||", 2) == 0)
	{
		*input = (*input) + 2;
		return (ft_strdup("||"));
	}
	else if (ft_strncmp(input[0], "|", 1) == 0)
	{
		*input = (*input) + 1;
		return (ft_strdup("|"));
	}
	ft_assert(0, "unexpected input on parse_pipe_or_opor");
	return (NULL);
}

char	*parse_operator(char **input)
{
	char	curr;

	curr = input[0][0];
	if (curr == '>')
		return (parse_operator_redirect_out(input));
	else if (curr == '<')
		return (parse_operator_redirect_in(input));
	else if (curr == '(')
		return (parse_operator_paren_open(input));
	else if (curr == ')')
		return (parse_operator_paren_close(input));
	else if (curr == '&')
		return (parse_operator_opand(input));
	else if (curr == '|')
		return (parse_operator_pipe_or_opor(input));
	ft_assert(0, "unexpected input on parse_operator");
	return (NULL);
}
