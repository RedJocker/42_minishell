/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 00:47:28 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:02:48 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memlib.h"
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/repl/shell/token/token_internal/token_internal.h"

t_token	*token_new(t_token_type type, char *content)
{
	t_token	*token;

	token = ft_calloc(CHAR_BYTE, sizeof(t_token));
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
