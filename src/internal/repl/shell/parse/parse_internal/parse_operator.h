/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 01:16:25 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/24 04:02:34 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_OPERATOR_H
# define PARSE_OPERATOR_H

enum e_operator
{
	OPERATOR_SIZE = 7,
};

typedef struct s_operator	t_operator;
struct s_operator
{
	char			*chr;
	char			*double_chr;
	char			*msg;
};

typedef struct s_operators	t_operators;
struct s_operators
{
	t_operator	operator[OPERATOR_SIZE];
};

#endif
