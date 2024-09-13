/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 16:25:12 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/13 03:09:31 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_INTERNAL_H
# define TOKEN_INTERNAL_H

# include "internal/repl/token/token.h"

t_token		*token_classify(char *str_token);
void		token_destroy(t_token *token);
t_token		*token_new(t_token_type type, char *content);
void		token_print(t_token *token);

#endif
