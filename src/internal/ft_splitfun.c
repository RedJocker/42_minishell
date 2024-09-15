/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitfun.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 01:57:38 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/14 05:17:36 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_extension.h"
#include "ft_util.h"

static size_t	count_splits(char const *str, t_pred_int should_split)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		if (!should_split(*str))
		{
			count++;
			while (*str && !should_split(*str))
				str++;
		}
		else
			str++;
	}
	return (count);
}

static void	skip_ch(char const **str, t_pred_int should_split)
{
	while (**str && should_split(**str))
		(*str)++;
}

static char	*next_token(char const **str, t_pred_int should_split)
{
	size_t	next_len;
	char	*token;

	skip_ch(str, should_split);
	next_len = 0;
	while ((*str)[next_len] && !should_split((*str)[next_len]))
		next_len++;
	token = ft_substr(*str, 0, next_len);
	*str += next_len;
	return (token);
}

char	**ft_splitfun(char const *str, t_pred_int should_split)
{
	char	**arr;
	size_t	arr_len;
	size_t	arr_i;

	if (!str)
		return (NULL);
	arr_len = count_splits(str, should_split) + 1;
	arr = malloc(arr_len * sizeof(char *));
	if (!arr)
		return (NULL);
	arr_i = 0;
	while (arr_i < arr_len - 1)
	{
		arr[arr_i] = next_token(&str, should_split);
		if (!arr[arr_i])
		{
			ft_strarr_free(arr);
			return (NULL);
		}
		arr_i++;
	}
	arr[arr_i] = NULL;
	return (arr);
}
