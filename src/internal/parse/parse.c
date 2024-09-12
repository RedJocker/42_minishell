/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 21:17:27 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/12 00:38:21 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "ft_ctype.h"
#include "ft_string.h"
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal/parse/parse_internal.h"

int	parse_word_find_len(char *input)
{
	int	l;

	l = 0;
	// TODO: improve len count on quoted cases
	while (1)
	{
		if (input[l] == '\0'
			|| parse_is_operator(input[l]) || ft_isspace(input[l]))
			return (l);
		else if (input[l] == '"')
			while (input[++l] && input[l] != '"')
				;
		else if (input[l] == '\'')
			while (input[++l] && input[l] != '\'')
				;
		else if (input[l] == '\\')
			l++;
		l += input[l] != '\0';
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
