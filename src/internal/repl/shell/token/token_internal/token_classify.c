/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_classify.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:54:34 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/16 15:14:00 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "ft_string.h"
#include "internal/default.h"
#include "internal/repl/shell/token/token.h"
#include "internal/repl/shell/token/token_internal/token_internal.h"

static bool	token_is_invalid_quote(char *str_token)
{
	char	open_quote;
	int		i;

	open_quote = 0;
	i = DEFAULT_INIT;
	while (str_token[++i])
	{
		if (open_quote > DEFAULT)
			open_quote = (str_token[i] != open_quote) * open_quote;
		else if (str_token[i] == '\'' || str_token[i] == '\"')
			open_quote = str_token[i];
	}
	return (open_quote > DEFAULT);
}

// TODO: classify remaining operators
t_token	*token_classify(char *str_token)
{
	if (!str_token)
		return (token_new(OP_EOF, "OP_EOF"));
	else if (ft_strncmp("\n", str_token, ft_strlen("\n")) == DEFAULT)
		return (token_new(OP_NEWLINE, "newline"));
	else if (ft_strncmp(">>", str_token, ft_strlen(">>")) == DEFAULT)
		return (token_new(OP_REDIRECT_OUT_APPND, str_token));
	else if (ft_strncmp(">", str_token, ft_strlen(">")) == DEFAULT)
		return (token_new(OP_REDIRECT_OUT_TRUNC, str_token));
	else if (ft_strncmp("<<", str_token, ft_strlen("<<")) == DEFAULT)
		return (token_new(OP_REDIRECT_IN_HEREDOC, str_token));
	else if (ft_strncmp("<", str_token, ft_strlen("<")) == DEFAULT)
		return (token_new(OP_REDIRECT_IN, str_token));
	else if (ft_strncmp("||", str_token, ft_strlen("||")) == DEFAULT)
		return (token_new(OP_OR, str_token));
	else if (ft_strncmp("|", str_token, ft_strlen("|")) == DEFAULT)
		return (token_new(OP_PIPE, str_token));
	else if (ft_strncmp("&&", str_token, ft_strlen("|")) == DEFAULT)
		return (token_new(OP_AND, str_token));
	else if (token_is_invalid_quote(str_token))
		return (token_new(INVALID, "newline"));
	return (token_new(WORD, str_token));
}
