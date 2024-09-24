/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 21:17:27 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/24 04:24:48 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "ft_ctype.h"
#include "ft_string.h"
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/repl/shell/parse/parse_internal/parse_internal.h"

static char	*parse_input_chr(char **input)
{
	char	chr;

	chr = input[DEFAULT][DEFAULT];
	while (chr == '\0' || ft_isspace(chr))
	{
		if (chr == '\0' || chr == '\n')
		{
			*input = (*input) + CHAR_BYTE;
			return (ft_strdup("\n"));
		}
		*input = (*input) + CHAR_BYTE;
		chr = input[DEFAULT][DEFAULT];
	}
	if (parse_is_operator(input[DEFAULT][DEFAULT]))
		return (parse_operator(input));
	else
		return (parse_word(input));
}

static char	**parse_split_input(char *input)
{
	t_arraylist	lst_str;
	char		*chr;
	char		**str_tokens;

	lst_str = ft_arraylist_new(free);
	if (!lst_str)
		return (NULL);
	while (WAIT)
	{
		chr = parse_input_chr(&input);
		if (!chr)
		{
			ft_arraylist_destroy(lst_str);
			return (NULL);
		}
		lst_str = ft_arraylist_add(lst_str, chr);
		if (!lst_str)
			return (NULL);
		if (ft_strncmp(chr, "\n", CHAR_BYTE) == DEFAULT)
			break ;
	}
	str_tokens = ft_lststr_to_arrstr(lst_str);
	ft_arraylist_destroy(lst_str);
	return (str_tokens);
}

char	**parse_input(char *input)
{
	char	**str_tokens;

	if (!input)
		str_tokens = ft_calloc(CHAR_BYTE, sizeof(char *));
	else
		str_tokens = parse_split_input(input);
	free(input);
	return (str_tokens);
}
