/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str_star.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:43:32 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/25 19:58:39 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <unistd.h>
#include "ft_assert.h"
#include "collection/ft_arraylist.h"
#include "ft_string.h"
#include "internal_bonus/repl/shell/runner/expand/expand_internal.h"

static int	has_star(char *str)
{
	char	open_quote;
	int		i;

	open_quote = 0;
	i = -1;
	while (str[++i])
	{
		if (!open_quote && str[i] == '*')
			return (1);
		else if (!open_quote && (str[i] == '\'' || str[i] == '\"'))
			open_quote = str[i];
		else if (open_quote == str[i])
			open_quote = 0;
	}
	return (0);
}

static int	star_match_helper(char *star_str, char *filename, char open_quote);

static int	star_match_helper_star_case(
	char *star_str, char *filename, char open_quote)
{
	int	any_of;
	int	i;

	i = -1;
	while (++i >= 0)
	{
		any_of = star_match_helper(star_str + 1, filename + i, open_quote);
		if (any_of)
			return (1);
		else if (filename[i] == '\0')
			return (0);
	}
	ft_assert(0, "unexpected line executed at star_match_helper_star_case");
	return (-1);
}

static int	star_match_helper(char *star_str, char *filename, char open_quote)
{
	if (*star_str == '\0')
		return (*filename == '\0');
	else if (open_quote && *star_str == open_quote)
		return (star_match_helper(star_str + 1, filename, 0));
	else if (!open_quote && (*star_str == '\"' || *star_str == '\''))
		return (star_match_helper(star_str + 1, filename, *star_str));
	else if (*filename == '\0' && *star_str != '*')
		return (0);
	else if ((*star_str != '*' || open_quote) && *star_str != *filename)
		return (0);
	else if ((*star_str != '*' || open_quote) && *star_str == *filename)
		return (star_match_helper(star_str + 1, filename + 1, open_quote));
	else if (*star_str == '*')
		return (star_match_helper_star_case(star_str, filename, open_quote));
	ft_assert(0, "unexpected line executed at star_match_helper");
	return (0);
}

static int	star_match(char *star_str, char *filename)
{
	ft_assert(filename && *filename != '.' && star_str,
		"expected only non hidden files and non null params at star_match");
	return (star_match_helper(star_str, filename, 0));
}

void	expand_str_star(char *str, t_arraylist *out_lst)
{
	t_arraylist		lst_files;
	int				i;
	int				is_found;

	if (!has_star(str))
		*out_lst = ft_arraylist_add(*out_lst, ft_strdup(str));
	else
	{
		lst_files_init(&lst_files);
		i = -1;
		is_found = 0;
		while (ft_arraylist_get(lst_files, ++i))
			if (star_match(str, ft_arraylist_get(lst_files, i)) && ++is_found)
				*out_lst = ft_arraylist_add(*out_lst, \
					ft_strdup(ft_arraylist_get(lst_files, i)));
		if (!is_found)
			*out_lst = ft_arraylist_add(*out_lst, ft_strdup(str));
		ft_arraylist_destroy(lst_files);
	}
}
