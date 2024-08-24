/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:54:34 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/24 02:06:46 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "ft_memlib.h"
#include "ft_string.h"
#include "minishell.h"
#include <stdio.h>

void	token_type_string(char out_str[23], t_token_type type)
{
	if (type == OP_REDIRECT_OUT_TRUNC)
		ft_strlcpy(out_str, "OP_REDIRECT_OUT_TRUNC", 22);
	else if (type == WORD)
		ft_strlcpy(out_str, "WORD", 5);
}

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

t_token	*token_new(t_token_type type, char *content)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->content = ft_strdup(content);
	if (!token->content)
	{
		free(token);
		return (NULL);
	}
	return (token);
}

void	token_destroy(t_token *token)
{
	free(token->content);
	free(token);
}

void	tokens_destroy(t_token **tokens)
{
	int	i;

	i = -1;
	while (tokens[++i])
		token_destroy(tokens[i]);
	free(tokens);
}

// TODO
t_token	*token_classify(char *str_token)
{
	if (ft_strncmp(">", str_token, 2) == 0)
		return (token_new(OP_REDIRECT_OUT_TRUNC, str_token));
	return (token_new(WORD, str_token));
}

t_token	**tokens_classify(char **arr_str_tokens)
{
	int		len;
	int		i;
	t_token	**arr_tokens;

	len = 0;
	while (arr_str_tokens[len])
		len++;
	arr_tokens = ft_calloc(len + 1, sizeof(t_token *));
	if (!arr_tokens)
		return (NULL);
	i = -1;
	while (++i < len)
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
