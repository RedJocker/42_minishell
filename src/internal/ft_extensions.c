/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extensions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 22:31:17 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/28 03:26:54 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdio.h"
#include "ft_memlib.h"
#include "collection/ft_arraylist.h"
#include "ft_string.h"

void	ft_assert(int booll, char *msg)
{
	char	*mem;

	mem = 0;
	if (!booll)
	{
		ft_puterr("assertion failed: ");
		ft_puterrl(msg);
		(void) *mem;
	}
}

char	**ft_strarr_cat(char **strarr0, char **strarr1)
{
	int		len0_len1[2];
	char	**ret_strarr;
	int		len_ret;
	int		i_j[2];

	if (!strarr1 && !strarr1)
		return (NULL);
	len0_len1[0] = 0;
	while (strarr1 && strarr0[len0_len1[0]])
		len0_len1[0]++;
	len0_len1[1] = 0;
	while (strarr1 && strarr1[len0_len1[1]])
		len0_len1[1]++;
	len_ret = len0_len1[0] + len0_len1[1];
	ret_strarr = ft_calloc(len_ret + 1, sizeof(char *));
	if (!ret_strarr)
		return (NULL);
	i_j[0] = -1;
	i_j[1] = 0;
	while (++i_j[0] < len0_len1[0])
		ret_strarr[i_j[1]++] = strarr0[i_j[0]];
	i_j[0] = -1;
	while (++i_j[0] < len0_len1[1])
		ret_strarr[i_j[1]++] = strarr1[i_j[0]];
	return (ret_strarr);
}

char	**ft_lststr_to_arrstr(t_arraylist lststr)
{
	char	**arrstr;
	int		arrstr_len;
	int		i;

	arrstr_len = (int) ft_arraylist_len(lststr) + 1;
	arrstr = ft_calloc(arrstr_len, sizeof(char *));
	if (!arrstr)
		return (NULL);
	i = -1;
	while (++i < arrstr_len - 1)
		arrstr[i] = ft_strdup((char *) ft_arraylist_get(lststr, i));
	return (arrstr);
}
