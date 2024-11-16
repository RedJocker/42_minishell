/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:55:46 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/16 04:10:14 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include "stringbuilder.h"
#include "ft_assert.h"
#include "ft_string.h"
#include "ft_ctype.h"
#include "ft_memlib.h"
#include "internal/default.h"
#include "internal/env/env.h"

static int	expand_str_dollar_question(t_stringbuilder *builder, \
				sig_atomic_t last_status_code)
{
	char	*env_value;

	env_value = ft_itoa(last_status_code);
	stringbuilder_addstr(builder, env_value);
	free(env_value);
	return (1);
}

static int	expand_str_dollar_variable(char *str, t_stringbuilder *builder,	\
				sig_atomic_t last_status_code)
{
	int		i;
	char	*env_key;
	char	*env_value;

	ft_assert(ft_strncmp(str, "$", CHAR_BYTE) == DEFAULT, "expected $");
	if (ft_strncmp(str, "$?", CHAR_BYTE + CHAR_BYTE) == DEFAULT)
		return (expand_str_dollar_question(builder, last_status_code));
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
