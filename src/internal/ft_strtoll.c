/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 23:01:35 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/22 02:40:15 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <string.h>

static long long	ft_parse_nb(const char *str, long long nb, int sign, \
						char **endptr);

long long	ft_strtoll(const char *str, char **endptr)
{
	long long	nb;
	int			sign;

	nb = 0;
	sign = 1;
	if (str == NULL)
	{
		if (endptr)
			*endptr = NULL;
		return (0);
	}
	return (ft_parse_nb(str, nb, sign, endptr));
}

static long long	ft_parse_nb(const char *str, long long nb, int sign, \
						char **endptr)
{
	while ((*str >= '\t' && *str <= '\r') || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign *= -1;
	while (*str >= '0' && *str <= '9')
	{
		if (sign == 1 && (nb > LLONG_MAX / 10 \
			|| (nb == LLONG_MAX / 10 && (*str - '0') > LLONG_MAX % 10)))
		{
			nb = LLONG_MAX;
			break ;
		}
		if (sign == -1 && (nb > -(LLONG_MIN / 10) \
			|| (nb == -(LLONG_MIN / 10) && (*str - '0') > -(LLONG_MIN % 10))))
		{
			nb = LLONG_MIN;
			break ;
		}
		nb = (nb * 10) + (*str++ - '0');
	}
	if (endptr)
		*endptr = (char *)str;
	return (nb * sign);
}
