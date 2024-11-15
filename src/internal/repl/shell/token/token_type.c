/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 20:51:53 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/14 23:51:29 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	token_type_is_redirect(t_token *token)
{
	return (token
		&& (token->type == OP_REDIRECT_OUT_TRUNC
			|| token->type == OP_REDIRECT_OUT_APPND
			|| token->type == OP_REDIRECT_IN
			|| token->type == OP_REDIRECT_IN_HEREDOC));
}

int	token_type_is_word(t_token *token)
{
	return (token && token->type == WORD);
}

int	token_type_is_either_nl_eof(t_token *token)
{
	return (token
		&& (token->type == OP_NEWLINE
			|| token->type == OP_EOF));
}

int	token_type_is_end_op(t_token *token)
{
	return (token
		&& (token->type == OP_EOF
			|| token->type == OP_NEWLINE
			|| token->type == OP_PIPE
			|| token->type == OP_AND
			|| token->type == OP_OR
			|| token->type == OP_PAREN_CLOSE));
}
