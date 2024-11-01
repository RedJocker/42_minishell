/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:55:46 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/31 23:48:55 by maurodri         ###   ########.fr       */
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

#include "ft_stdio.h"

int	icompare_str(char *str0, char *str1)
{
	int isalternate_case;
	int isalternate_alpha;
	ft_printf("comp %s %s: ", str0, str1);
	while (*str0 && *str1)
	{
		// TODO: add weird behaviour when comparing sybols agains letters
		// mini#sg < mini%sg < mini.sg < minishell < mini#si < mini%si < mini.si
		isalternate_case = (ft_islower(*str0) && ft_isupper(*str1)) \
			|| (ft_isupper(*str0) && ft_islower(*str1));
		isalternate_alpha = (ft_isalpha(*str0) && !ft_isalpha(*str1)) \
			|| (!ft_isalpha(*str0) && ft_isalpha(*str1));
		if (*str0 == *str1)
			;
		else if (isalternate_case)
		{
			ft_printf("alternate %d\n", (ft_isupper(*str0)));
			return (ft_isupper(*str0));
		}
		else if (isalternate_alpha)
		{
			str0 += !ft_isalpha(*str0);
			str1 += !ft_isalpha(*str1);
			continue ;
		}
		else
		{
			ft_printf("else %d\n", (*str0 - *str1));
			return (*str0 - *str1);
		}
			
		str0++;
		str1++;
	}
	ft_printf("outside %d\n", (*str0 - *str1));
	return *str0 - *str1;
}

void	ft_arraylist_sort(t_arraylist lst, t_intbifun compare_fun)
{
	size_t	i;
	size_t	top;
	
	top = ft_arraylist_len(lst);
	while (--top > 0)
	{
		i = 0;
		while (++i <= top)
		{
			if (compare_fun(ft_arraylist_get(lst, i - 1), \
							ft_arraylist_get(lst, i)) > 0)
				ft_arraylist_swap(lst, i - 1, i);
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
		free(cwd);
		if (dp == NULL)
		{
			// TODO: error?
		}
		lst_files = ft_arraylist_new(free);
		dirp = readdir(dp);
		while (dirp)
		{
			if (dirp->d_name[0] != '.')
				lst_files = ft_arraylist_add(lst_files, ft_strdup(dirp->d_name));
			dirp = readdir(dp);
		}
		//              sort files case insensitive
		ft_arraylist_sort(lst_files, (t_intbifun) icompare_str);
		i = -1;
		while (ft_arraylist_get(lst_files, ++i))
			if (star_match(str, ft_arraylist_get(lst_files, i)))
				//    add files that match to out_lst
				ft_arraylist_add(*out_lst, \
					ft_strdup(ft_arraylist_get(lst_files, i)));
		ft_arraylist_destroy(lst_files);
		closedir(dp);
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
