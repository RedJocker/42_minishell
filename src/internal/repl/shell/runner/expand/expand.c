/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:38:29 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/03 02:13:34 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_assert.h"
#include "ft_ctype.h"
#include "ft_string.h"
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal/default.h"
#include "stringbuilder.h"
#include "internal/env/env.h"
#include "internal/repl/shell/runner/expand/expand.h"
#include "internal/repl/shell/runner/expand/expand_internal.h"

#include "internal/ft_extension.h" // TODO: Remove this after finish the project.

// return len parsed
static int	expand_dollar(char *str, t_stringbuilder *builder,	\
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

// return len parsed
static int	expand_dollar_split(char *str, t_expansion_state *state, \
				sig_atomic_t last_status_code)
{
	int		i;
	char	**split;
	char	*temp;
	int		j;
	
	i = expand_dollar(str, &state->builder, last_status_code);
	temp = stringbuilder_build(state->builder);
	split = ft_splitfun(temp, (t_pred_int) ft_isspace);
	free(temp);
	j = -1;
	if (split[0] != NULL)
		while (split[++j + 1])
			state->lst_new_args = ft_arraylist_add(
					state->lst_new_args, ft_strdup(split[j]));
	state->builder = stringbuilder_new();
	if (split[0] != NULL)
		stringbuilder_addstr(&state->builder, split[j]);
	ft_strarr_free(split);
	return (i);
}

static void	expand_str_noquote(
	char *str, sig_atomic_t last_status_code, t_expansion_state *state)
{
	if (str[state->cur_ch] == '$')
		state->cur_ch += expand_dollar_split(
				str + state->cur_ch, state, last_status_code);
	else if (str[state->cur_ch] == '\'' || str[state->cur_ch] == '\"')
		state->open_quote = str[state->cur_ch];
	else
		state->builder = stringbuilder_addchar(
				state->builder, str[state->cur_ch]);
}

static void	expand_str(
	char *str, sig_atomic_t last_status_code, t_expansion_state *state)
{
	state->builder = stringbuilder_new();
	state->open_quote = 0;
	state->cur_ch = -1;
	while (str[++(state->cur_ch)])
	{
		if (state->open_quote == 0)
			expand_str_noquote(str, last_status_code, state);
		else
		{
			if (str[state->cur_ch] == state->open_quote)
				state->open_quote = 0;
			else if (state->open_quote == '\"' && str[state->cur_ch] == '$')
				state->cur_ch += expand_dollar(
						str + state->cur_ch, &state->builder, last_status_code);
			else
				state->builder = stringbuilder_addchar(
						state->builder, str[state->cur_ch]);
		}
	}
	state->res = stringbuilder_build(state->builder);
	state->lst_new_args = (ft_arraylist_add(state->lst_new_args, state->res));
}

void	expand(t_command cmd, sig_atomic_t last_status_code)
{
	t_expansion_state	state;
	int					i;

	if (cmd->type != CMD_SIMPLE)
		return ;
	state.lst_new_args = ft_arraylist_new(free);
	i = -1;
	while (cmd->simple->cmd_argv[++i])
		expand_str(
			cmd->simple->cmd_argv[i], last_status_code, &state);
	ft_strarr_free(cmd->simple->cmd_argv);
	cmd->simple->cmd_argv = ft_lststr_to_arrstr(state.lst_new_args);
	ft_arraylist_destroy(state.lst_new_args);
	cmd->simple->cmd_argc = 0;
	while (cmd->simple->cmd_argv[cmd->simple->cmd_argc])
		cmd->simple->cmd_argc++;
}
