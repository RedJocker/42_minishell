/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extension.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 01:55:16 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/14 05:17:58 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_EXTENSION_H
# define FT_EXTENSION_H

typedef int	(*t_pred_char)(char);
typedef int	(*t_pred_int) (int);

char	**ft_splitfun(char const *str, t_pred_int should_split);

#endif
