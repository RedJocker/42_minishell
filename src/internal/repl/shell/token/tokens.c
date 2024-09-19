/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 16:22:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/19 08:41:01 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "ft_memlib.h"
#include "internal/repl/shell/token/token_internal.h"

t_token	**tokens_classify(char **arr_str_tokens, int *out_len)
{
	int		i;
	t_token	**arr_tokens;

	*out_len = 0;
	while (arr_str_tokens[*out_len])
		(*out_len)++;
	*out_len = (*out_len == 0) * 1 + (*out_len != 0) * *out_len;
	arr_tokens = ft_calloc(*out_len + 1, sizeof(t_token *));
	if (!arr_tokens)
		return (NULL);
	i = -1;
	while (++i < *out_len)
	{
		arr_tokens[i] = token_classify(arr_str_tokens[i]);
		if (!arr_tokens[i])
		{
			tokens_destroy(arr_tokens);
			return (NULL);
		}
	}
	return (arr_tokens);
}

void	tokens_destroy(t_token **tokens)
{
	int	i;

	i = -1;
	while (tokens[++i])
		token_destroy(tokens[i]);
	free(tokens);
}

void	tokens_print(t_token **tokens)
{
	int	i;

	i = -1;
	printf("{\n");
	while (tokens[++i])
	{
		printf("\t");
		token_print(tokens[i]);
		printf("\n");
	}
	printf("}\n");
}
