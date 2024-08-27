/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 19:15:47 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/27 00:08:42 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_INTERNAL_H
# define COMMAND_INTERNAL_H

# include "internal/command/command.h"

t_command	command_new(t_command_type type, char *type_str);
void		command_free(t_command cmd);
int			command_operator_idx(t_token **tokens, int tokens_len);
t_command	command_invalid_new(char *message);
void		command_invalid_destroy(t_command cmd);
t_command	command_simple_new(
				t_token **tokens, int fst_token_idx, int endtoken_idx);
void		command_simple_destroy(t_command cmd);
int			command_simple_is_invalid(
				t_token **tokens, int fst_token_idx, int *endtoken_idx);

#endif
