/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extensions.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 22:32:31 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/28 03:27:01 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
    functions that could be added to libftx.
    using this to avoid versioning issues on small updates.

	possibly needs fixing
*/

#ifndef FT_EXTENSIONS_H
# define FT_EXTENSIONS_H

# include "collection/ft_arraylist.h"

void	ft_assert(int booll, char *msg);
char	**ft_strarr_cat(char **strarr0, char **strarr1);
char	**ft_lststr_to_arrstr(t_arraylist lststr);

#endif
