/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_icompare_str.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:12:27 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/25 20:01:41 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <unistd.h>
#include "ft_string.h"
#include "ft_ctype.h"
#include "internal_bonus/repl/shell/runner/expand/expand_internal.h"

static char	ch_tolower(unsigned int i, char ch)
{
	(void) i;
	if (ft_isupper(ch))
		return (ch + 32);
	else
		return (ch);
}

static int	alternate_case(char *str0, char *str1)
{
	char	*lower[2];
	int		temp;

	lower[0] = ft_strmapi(str0, ch_tolower);
	lower[1] = ft_strmapi(str1, ch_tolower);
	temp = expand_icompare_str(lower[0], lower[1]);
	free(lower[0]);
	free(lower[1]);
	if (temp == 0)
		return (ft_isupper(*str0));
	else
		return (temp);
}

static int	both_not_alpha_case(char *str0, char *str1)
{
	int		temp;

	temp = expand_icompare_str(str0 + 1, str1 + 1);
	if (temp == 0)
		return (*str0 - *str1);
	else
		return (temp);
}

int	expand_icompare_str(char *str0, char *str1)
{
	int		isalternate_case;
	int		isalternate_alpha;

	while (*str0 && *str1)
	{
		isalternate_case = ((ft_islower(*str0) && ft_isupper(*str1))
				|| (ft_isupper(*str0) && ft_islower(*str1)));
		isalternate_alpha = ((ft_isalpha(*str0) && !ft_isalpha(*str1))
				|| (!ft_isalpha(*str0) && ft_isalpha(*str1)));
		if (*str0 == *str1 && str0++ && str1++)
			;
		else if (isalternate_case)
			return (alternate_case(str0, str1));
		else if (isalternate_alpha)
		{
			str0 += !ft_isalpha(*str0);
			str1 += !ft_isalpha(*str1);
		}
		else if (!ft_isalpha(*str0) && !ft_isalpha(*str1))
			return (both_not_alpha_case(str0, str1));
		else
			return (*str0 - *str1);
	}
	return (*str0 - *str1);
}
