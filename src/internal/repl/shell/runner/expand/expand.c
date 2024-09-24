/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:38:29 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/24 20:49:36 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "ft_string.h"
#include "stringbuilder.h"
#include "ft_ctype.h"
#include "ft_assert.h"
#include "ft_memlib.h"
#include "ft_util.h"
#include "internal/ft_extension.h"

static char	*env_mock(char *s)
{
	(void) s;
	return (ft_strdup("abc def"));
}

static int	runner_cmd_expand_dollar(char *str, t_stringbuilder *builder, \
		sig_atomic_t last_status_code)
{
	int		i;
	char	*var_name;
	char	*var_value;

	ft_assert(ft_strncmp(str, "$", 1) == 0, "expected $");
	if (ft_strncmp(str, "$?", 2) == 0)
	{
		var_value = ft_itoa(last_status_code);
		stringbuilder_addstr(builder, var_value);
		free(var_value);
		return (1);
	}
	i = 1;
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		i++;
		while (ft_isalnum(str[i]) || str[i] == '_')
			i++;
	}
	var_name = ft_calloc(i, sizeof(char));
	ft_strlcpy(var_name, str + 1, i);
	// TODO: look expansion on env
	var_value = env_mock(var_name);
	free(var_name);
	stringbuilder_addstr(builder, var_value);
	free(var_value);
	return (i - 1);
}

static int	runner_cmd_expand_dollar_split(char *str, t_stringbuilder *builder, \
		t_arraylist *lst_new_args, sig_atomic_t last_status_code)
{
	int		i;
	char	**split;
	char	*temp;
	int		j;

	i = runner_cmd_expand_dollar(str, builder, last_status_code);
	temp = stringbuilder_build(*builder);
	split = ft_splitfun(temp, (t_pred_int) ft_isspace);
	free(temp);
	j = -1;
	while (split[++j + 1])
		*lst_new_args = ft_arraylist_add(*lst_new_args, ft_strdup(split[j]));
	*builder = stringbuilder_new();
	stringbuilder_addstr(builder, split[j]);
	ft_strarr_free(split);
	return (i);
}

static void	runner_cmd_expand_str(char *str, sig_atomic_t last_status_code, \
			t_arraylist *lst_new_args)
{
	t_stringbuilder	builder;
	char			*res;
	char			open_quote;
	int				i;

	builder = stringbuilder_new();
	open_quote = 0;
	i = -1;
	while (str[++i])
	{
		if (open_quote == 0)
		{
			if (str[i] == '$')
				i += runner_cmd_expand_dollar_split(str + i, &builder, lst_new_args, last_status_code);
			else if (str[i] == '\'' || str[i] == '\"')
				open_quote = str[i];
			else
				builder = stringbuilder_addchar(builder, str[i]);
		}
		else
		{
			if (str[i] == open_quote)
				open_quote = 0;
			else if (open_quote == '\"' && str[i] == '$')
				i += runner_cmd_expand_dollar(str + i, &builder, last_status_code);
			else
				builder = stringbuilder_addchar(builder, str[i]);
		}
	}
	res = stringbuilder_build(builder);
	*lst_new_args = (ft_arraylist_add(*lst_new_args, res));
}

void	runner_cmd_expand(t_command cmd, sig_atomic_t last_status_code)
{
	t_arraylist	lst_new_args;
	int			i;

	if (cmd->type != CMD_SIMPLE)
		return ;
	lst_new_args = ft_arraylist_new(free);
	i = -1;
	while (cmd->simple->cmd_argv[++i])
		runner_cmd_expand_str(
			cmd->simple->cmd_argv[i], last_status_code, &lst_new_args);
	// TODO: improve allocation error handling
	ft_strarr_free(cmd->simple->cmd_argv);
	cmd->simple->cmd_argv = ft_lststr_to_arrstr(lst_new_args);
	ft_arraylist_destroy(lst_new_args);
	cmd->simple->cmd_argc = 0;
	while (cmd->simple->cmd_argv[cmd->simple->cmd_argc])
		cmd->simple->cmd_argc++;
}
