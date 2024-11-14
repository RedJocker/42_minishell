/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 20:51:53 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/13 20:52:21 by maurodri         ###   ########.fr       */
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
