/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 16:22:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:02:20 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal_bonus/default.h"
#include "internal_bonus/repl/shell/token/token.h"
#include "internal_bonus/repl/shell/token/token_internal/token_internal.h"

void	tokens_destroy(t_token **tokens)
{
	int	i;

	i = DEFAULT_INIT;
	while (tokens[++i])
		token_destroy(tokens[i]);
	free(tokens);
}

t_token	**tokens_classify(char **arr_str_tokens, int *out_len)
{
	int		i;
	t_token	**arr_tokens;

	*out_len = DEFAULT;
	while (arr_str_tokens[*out_len])
		(*out_len)++;
	*out_len = (*out_len == DEFAULT) * CHAR_BYTE \
		+ (*out_len != NULL_BYTE) * *out_len;
	arr_tokens = ft_calloc(*out_len + NULL_BYTE, sizeof(t_token *));
	if (!arr_tokens)
		return (NULL);
	i = DEFAULT_INIT;
	while (++i < *out_len)
	{
		arr_tokens[i] = token_classify(arr_str_tokens[i]);
		if (!arr_tokens[i])
		{
			tokens_destroy(arr_tokens);
			return (NULL);
		}
	}
	ft_strarr_free(arr_str_tokens);
	return (arr_tokens);
}
