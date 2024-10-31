/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:55:46 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/30 21:11:47 by maurodri         ###   ########.fr       */
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

// return len parsed
static int	expand_str_dollar_variable(char *str, t_stringbuilder *builder,	\
				sig_atomic_t last_status_code)
{
	int		i;
	char	*env_key;
	char	*env_value;

	ft_assert(ft_strncmp(str, "$", CHAR_BYTE) == DEFAULT, "expected $");
	if (ft_strncmp(str, "$?", CHAR_BYTE + CHAR_BYTE) == DEFAULT)
	{
		env_value = ft_itoa(last_status_code);
		stringbuilder_addstr(builder, env_value);
		free(env_value);
		return (1);
	}
	i = DEFAULT;
	if (ft_isalpha(str[++i]) || str[i] == '_')
		while (ft_isalnum(str[i]) || str[i] == '_')
			i++;
	else
	{
		*builder = stringbuilder_addchar(*builder, '$');
		return (i - NULL_BYTE);
	}
	env_key = ft_calloc(i, sizeof(char));
	ft_strlcpy(env_key, str + NULL_BYTE, i);
	env_value = env_get_value(env_key);
	free(env_key);
	if (!env_value)
		return (i - NULL_BYTE);
	stringbuilder_addstr(builder, env_value);
	free(env_value);
	return (i - NULL_BYTE);
}

int		has_star(char *str)
{
	// TODO: improve with quote consideration
	return (ft_strchr(str, '*') != NULL);
}

int	icompare_str(char *str1, char *str2)
{
	while (*str1 && *str2)
	{
		if (ft_tolower(*str1) != (ft_tolower(*str2)))
			return (ft_tolower(*str1) - (ft_tolower(*str2)));
		str1++;
		str2++;
	}
	return (ft_tolower(*str1) - (ft_tolower(*str2)));
}

void	ft_arraylist_sort(t_arraylist lst, t_intbifun compare_fun)
{
	int		i;
	int		next_i;

	i = -1;
	while (ft_arraylist_get(lst, ++i))
	{
		next_i = i + 1;
		while (ft_arraylist_get(lst, ++next_i))
		{
			if (compare_fun(ft_arraylist_get(lst, i), \
							ft_arraylist_get(lst, next_i)) > 0)
				ft_arraylist_swap(lst, i, next_i);
		}
	}
}

int star_match(char *star_str, char *filename)
{
	// hide hidden files
	//TODO make star match
	return (*filename != '.' && star_str);
}

void	expand_str_star(char *str, t_arraylist *out_lst)
{
	char			*cwd;
	DIR				*dp;
	struct dirent	*dirp;
	t_arraylist		lst_files;
	char			**arr_files;
	int				i;
	// TODO: retrieve files current directory
	//       verify if has pattern
	if (!has_star(str))
	//           if not
	//              add str to out_lst
		*out_lst = ft_arraylist_add(*out_lst, ft_strdup(str));
	else
	{
		//       if yes retrieve files
		cwd = NULL;
		cwd = getcwd(cwd, 0);
		dp = opendir(cwd);
		if (dp == NULL)
		{
			// TODO: error?
		}
		lst_files = ft_arraylist_new(free);
		dirp = readdir(dp);
		while (dirp)
		{
			lst_files = ft_arraylist_add(lst_files, ft_strdup(dirp->d_name));
			dirp = readdir(dp);
		}
		//              sort files case insensitive
		ft_arraylist_sort(lst_files, (t_intbifun) icompare_str);
		arr_files = ft_lststr_to_arrstr(lst_files);
		ft_arraylist_destroy(lst_files);
		//ft_strarr_printfd(arr_files, 0);
		i = -1;
		while (arr_files[++i])
			if (star_match(str, arr_files[i]))
				//    add files that match to out_lst
				ft_arraylist_add(*out_lst, ft_strdup(arr_files[i]));
	}
}

void	expand_str_dollar(char **strptr, sig_atomic_t last_status_code)
{
	t_stringbuilder	builder;
	int				i;
	int				is_singlequoted;

	builder = stringbuilder_new();
	is_singlequoted = 0;
	i = -1;
	while ((*strptr)[++i])
	{
		if (!is_singlequoted && (*strptr)[i] == '\'')
			is_singlequoted = '\'';
		else if (is_singlequoted && (*strptr)[i] == '\'')
			is_singlequoted = 0;
		if (!is_singlequoted && (*strptr)[i] == '$')
			i += expand_str_dollar_variable((*strptr) + i, \
				&builder, last_status_code);
		else
			builder = stringbuilder_addchar(builder, (*strptr)[i]);
	}
	free((*strptr));
	*strptr = stringbuilder_build(builder);
}

void	expand_str_remove_quote(char **strptr)
{
	t_stringbuilder	builder;
	int				i;
	int				open_quote;

	builder = stringbuilder_new();
	open_quote = 0;
	i = -1;
	while ((*strptr)[++i])
	{
		if (!open_quote && ((*strptr)[i] == '\'' || (*strptr)[i] == '\"'))
			open_quote = (*strptr)[i];
		else if (open_quote && open_quote == (*strptr)[i])
			open_quote = 0;
		else
			builder = stringbuilder_addchar(builder, (*strptr)[i]);
	}
	free((*strptr));
	*strptr = stringbuilder_build(builder);
}
