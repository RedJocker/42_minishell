/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extension.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 01:55:16 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/30 01:41:14 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_EXTENSION_H
# define FT_EXTENSION_H

# include "collection/ft_arraylist.h"
# include "ft_util.h"

typedef int	(*t_pred_char)(char);
typedef int	(*t_pred_int) (int);

char		**ft_splitfun(char const *str, t_pred_int should_split);
long long	ft_strtoll(const char *str, char **endptr);
void		ft_arraylist_sort(t_arraylist lst, t_intbifun compare_fun);

#endif
