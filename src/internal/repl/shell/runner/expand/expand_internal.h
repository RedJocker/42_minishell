/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 21:10:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/19 15:13:46 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_INTERNAL_H
# define EXPAND_INTERNAL_H

# include "stringbuilder.h"
# include "collection/ft_arraylist.h"

typedef struct s_expansion_state
{
	t_stringbuilder	builder;
	char			*res;
	char			open_quote;
	int				had_quote;
	int				cur_ch;
	t_arraylist		lst_new_args;
}	t_expansion_state;

#endif
