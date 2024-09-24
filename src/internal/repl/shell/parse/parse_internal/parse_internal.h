/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:09:06 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/23 12:47:24 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_INTERNAL_H
# define PARSE_INTERNAL_H

# include <stdbool.h>

bool		parse_is_operator(char operator);
char	*parse_word(char **input);
char	*parse_operator_newline(char **input);
char	*parse_operator(char **input);

#endif
