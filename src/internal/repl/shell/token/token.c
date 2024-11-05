/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 16:22:24 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/04 22:36:38 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/repl/shell/token/token.h"
#include "internal/repl/shell/token/token_internal/token_internal.h"

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

// TODO: Remove after finish the project
#include <stdio.h>
#include "ft_string.h"
// TODO: Remove after finish the project
void	token_type_string(char out_str[23], t_token_type type)
{
	if (type == OP_REDIRECT_OUT_TRUNC)
		ft_strlcpy(out_str, "OP_REDIRECT_OUT_TRUNC", 22);
	else if (type == OP_REDIRECT_OUT_APPND)
		ft_strlcpy(out_str, "OP_REDIRECT_OUT_APPND", 22);
	else if (type == OP_REDIRECT_IN)
		ft_strlcpy(out_str, "OP_REDIRECT_IN", 15);
	else if (type == OP_REDIRECT_IN_HEREDOC)
		ft_strlcpy(out_str, "OP_REDIRECT_IN_HEREDOC", 23);
	else if (type == OP_PIPE)
		ft_strlcpy(out_str, "OP_PIPE", 8);
	else if (type == OP_AND)
		ft_strlcpy(out_str, "OP_AND", 7);
	else if (type == OP_OR)
		ft_strlcpy(out_str, "OP_OR", 6);
	else if (type == OP_NEWLINE)
		ft_strlcpy(out_str, "OP_NEW_LINE", 12);
	else if (type == OP_PAREN_OPEN)
		ft_strlcpy(out_str, "OP_PAREN_OPEN", 14);
	else if (type == OP_PAREN_CLOSE)
		ft_strlcpy(out_str, "OP_PAREN_CLOSE", 15);
	else if (type == WORD)
		ft_strlcpy(out_str, "WORD", 5);
	else if (type == OP_EOF)
		ft_strlcpy(out_str, "OP_EOF", 7);
	else if (type == INVALID)
		ft_strlcpy(out_str, "INVALID", 8);
}

// TODO: Remove after finish the project
void	token_print(t_token *token)
{
	char	type_str[23];

	token_type_string(type_str, token->type);
	printf("Token(type: %s, content: %s)", type_str, token->content);
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
