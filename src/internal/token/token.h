/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 00:52:37 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/24 01:12:44 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "minishell.h"

t_token	**tokens_classify(char **arr_str_tokens);
void	tokens_destroy(t_token **tokens);
void	tokens_print(t_token **tokens);

#endif
