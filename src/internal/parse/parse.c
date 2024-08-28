/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 21:17:27 by dande-je          #+#    #+#             */
/*   Updated: 2024/08/28 03:32:13 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "ft_string.h"
#include "ft_memlib.h"
#include "ft_stdio.h"
#include "internal/token/token.h"
#include "ft_ctype.h"
#include "internal/ft_extensions.h"

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

int	parse_word_find_len(char *input)
{
	int	l;

	l = 0;
	while (1)
	{
		if (input[l] == '\0' || parse_is_operator(input[l]) || ft_isspace(input[l]))
			return (l);
		else if (input[l] == '"')
		{
			while (input[++l] != '"')
				;
		}
		else if (input[l] == '\'')
		{
			while (input[++l] != '\'')
				;
		}
		else if (input[l] == '\\')
		{
			if (input[++l] == '\0')
				continue ;
		}
		l++;
	}
}

char	*parse_word(char **input)
{
	int		word_len;
	char	*word;

	word_len = parse_word_find_len(*input);
	word = ft_calloc(word_len + 1, sizeof(char));
	ft_strlcpy(word, *input, word_len + 1);
	*input = *input + word_len;
	return (word);
}

char	*parse_any(char **input)
{
	char	curr;

	curr = input[0][0];
	while (curr == '\0' || ft_isspace(curr))
	{
		if (curr == '\0' || curr == '\n')
			return (parse_operator_newline(input));
		*input = (*input) + 1;
		curr = input[0][0];
	}
	if (parse_is_operator(input[0][0]))
		return (parse_operator(input));
	else
		return (parse_word(input));
}

char	**parse_split_input(char *input)
{
	t_arraylist	lst_pieces;
	char		*input_piece;
	char		**str_tokens;

	lst_pieces = ft_arraylist_new(free);
	if (!lst_pieces)
		return (NULL);
	while (1)
	{
		input_piece = parse_any(&input);
		if (!input_piece)
		{
			ft_arraylist_destroy(lst_pieces);
			return (NULL);
		}
		lst_pieces = ft_arraylist_add(lst_pieces, input_piece);
		if (!lst_pieces)
			return (NULL);
		if (ft_strncmp(input_piece, "\n", 1) == 0)
			break ;
	}
	str_tokens = ft_lststr_to_arrstr(lst_pieces);
	ft_arraylist_destroy(lst_pieces);
	return (str_tokens);
}

char	**parse(char *input)
{
	char	**arr;

	if (!input)
		arr = ft_calloc(1, sizeof(char *));
	else
		arr = parse_split_input(input);
	return (arr);
}
