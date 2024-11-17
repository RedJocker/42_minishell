/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:06:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:07:44 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "ft_assert.h"
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/repl/shell/parse/parse_internal/parse_operator.h"

static t_operators	operators_init(void);
static char			*get_operator_type(char **input, \
						const t_operators operators, int operators_len);

bool	parse_is_operator(char chr)
{
	return (chr == '>' || chr == '<' || chr == '(' || chr == ')' || \
		chr == '&' || chr == '{' || chr == '|');
}

char	*parse_operator(char **input)
{
	int					operators_len;
	const t_operators	operators = operators_init();

	operators_len = DEFAULT_INIT;
	while (operators.operator[++operators_len].chr)
	{
		if (ft_strncmp(*input, operators.operator[operators_len].chr, \
			CHAR_BYTE) == DEFAULT)
		{
			return (get_operator_type(input, operators, operators_len));
			break ;
		}
	}
	ft_assert(DEFAULT, "unexpected input on parse_operator");
	return (NULL);
}

static char	*get_operator_type(char **input, const t_operators operators, \
				int operators_len)
{
	if (operators.operator[operators_len].double_chr && \
		ft_strncmp(*input, operators.operator[operators_len].double_chr, \
		CHAR_BYTE + CHAR_BYTE) == DEFAULT)
	{
		*input = (*input) + CHAR_BYTE + CHAR_BYTE;
		return (ft_strdup(operators.operator[operators_len].double_chr));
	}
	else if (ft_strncmp(*input, operators.operator[operators_len].chr, \
		CHAR_BYTE) == DEFAULT)
	{
		*input = (*input) + CHAR_BYTE;
		return (ft_strdup(operators.operator[operators_len].chr));
	}
	ft_assert(DEFAULT, operators.operator[operators_len].msg);
	return (NULL);
}

static t_operators	operators_init(void)
{
	return ((t_operators){.operator = {
			{">", ">>", "unexpected input on parse_redirect_out"},
			{"<", "<<", "unexpected input on parse_redirect_in"},
			{"(", NULL, "unexpected input on parse_paren_open"},
			{")", NULL, "unexpected input on parse_paren_close"},
			{"&", "&&", "unexpected input on parse_opand"},
			{"|", "||", "unexpected input on parse_pipe_or_opor"},
			{NULL, NULL, NULL},
		}});
}
