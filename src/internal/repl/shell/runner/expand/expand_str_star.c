/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str_star.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:43:32 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/16 00:31:42 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "expand_internal.h"
#include "ft_assert.h"
#include "ft_string.h"
#include "ft_ctype.h"
#include "ft_util.h"
#include "internal/default.h"
#include "ft_memlib.h"
#include "internal/env/env.h"
#include "internal/ft_extension.h"
#include <unistd.h>
#include <dirent.h>

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

static int	star_match_helper(char *star_str, char *filename)
{
	int	any_of;
	int	i;

	if (*star_str == '\0')
		return (*filename == '\0');
	else if (*filename == '\0')
		return (0);
	else if (*star_str != '*' && *star_str != *filename)
		return (0);
	else if (*star_str != '*' && *star_str == *filename)
		return (star_match_helper(star_str + 1, filename + 1));
	else if (*star_str == '*')
	{
		i = 0;
		while (++i)
		{
			any_of = star_match_helper(star_str + 1, filename + i);
			if (any_of)
				return (1);
			else if (filename[i] == '\0')
				return (0);
		}
	}
	ft_assert(0, "unexpected line executed at star_match_helper");
	return (0);
}

static int	star_match(char *star_str, char *filename)
{
	ft_assert(filename && *filename != '.' && star_str,
		"expected only non hidden files and non null params at star_match");
	return (star_match_helper(star_str, filename));
}

static void	lst_files_init(t_arraylist	*lst_files)
{
	char			*cwd;
	DIR				*dp;
	struct dirent	*dirp;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	dp = opendir(cwd);
	free(cwd);
	*lst_files = ft_arraylist_new(free);
	dirp = readdir(dp);
	while (dirp)
	{
		if (dirp->d_name[0] != '.')
			*lst_files = ft_arraylist_add(*lst_files, \
			ft_strdup(dirp->d_name));
		dirp = readdir(dp);
	}
	closedir(dp);
	ft_arraylist_sort(*lst_files, (t_intbifun) expand_icompare_str);
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
