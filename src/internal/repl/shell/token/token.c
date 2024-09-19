/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:54:34 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/19 08:40:47 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include "ft_memlib.h"
#include "ft_string.h"
#include "internal/repl/shell/token/token.h"

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
	else if (type == OP_NEWLINE)
		ft_strlcpy(out_str, "OP_NEW_LINE", 12);
	else if (type == WORD)
		ft_strlcpy(out_str, "WORD", 5);
	else if (type == OP_EOF)
		ft_strlcpy(out_str, "OP_EOF", 7);
	else if (type == INVALID)
		ft_strlcpy(out_str, "INVALID", 8);
}

void	token_print(t_token *token)
{
	char	type_str[23];

	token_type_string(type_str, token->type);
	printf("Token(type: %s, content: %s)", type_str, token->content);
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

bool	token_is_invalid_quote(char *str_token)
{
	char	open_quote;
	int		i;

	open_quote = 0;
	i = -1;
	while (str_token[++i])
	{
		if (open_quote > 0)
			open_quote = (str_token[i] != open_quote) * open_quote;
		else if (str_token[i] == '\'' || str_token[i] == '\"')
			open_quote = str_token[i];
	}
	return (open_quote > 0);
}

// TODO: classify remaining operators
t_token	*token_classify(char *str_token)
{
	if (!str_token)
		return (token_new(OP_EOF, "OP_EOF"));
	else if (ft_strncmp("\n", str_token, 2) == 0)
		return (token_new(OP_NEWLINE, "newline"));
	else if (ft_strncmp(">>", str_token, 3) == 0)
		return (token_new(OP_REDIRECT_OUT_APPND, str_token));
	else if (ft_strncmp(">", str_token, 2) == 0)
		return (token_new(OP_REDIRECT_OUT_TRUNC, str_token));
	else if (ft_strncmp("<", str_token, 2) == 0)
		return (token_new(OP_REDIRECT_IN, str_token));
	else if (token_is_invalid_quote(str_token))
		return (token_new(INVALID, "newline"));
	return (token_new(WORD, str_token));
}
