/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:02:50 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/13 22:18:42 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ctype.h"
#include "ft_memlib.h"
#include "ft_string.h"
#include "internal/default.h"
#include "internal/repl/shell/parse/parse_internal/parse_internal.h"

static int	parse_word_find_len(char *input)
{
	int	i;

	i = DEFAULT;
	while (WAIT)
	{
		if (input[i] == '\0' || parse_is_operator(input[i]) \
			|| ft_isspace(input[i]))
			break ;
		else if (input[i] == '"')
			while (input[++i] && input[i] != '"')
				;
		else if (input[i] == '\'')
			while (input[++i] && input[i] != '\'')
				;
		else if (input[i] == '\\')
			i++;
		i += input[i] != '\0';
	}
	return (i);
}

char	*parse_word(char **input)
{
	int		word_len;
	char	*word;

	word_len = parse_word_find_len(*input);
	word = ft_calloc(word_len + NULL_BYTE, sizeof(char));
	ft_strlcpy(word, *input, word_len + NULL_BYTE);
	*input = *input + word_len;
	return (word);
}
