/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_destroy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 00:31:43 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:02:33 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "internal_bonus/repl/shell/token/token_internal/token_internal.h"

void	token_destroy(t_token *token)
{
	free(token->content);
	free(token);
}
