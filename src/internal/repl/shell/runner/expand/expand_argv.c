/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:20:18 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/25 22:08:50 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"
#include "ft_util.h"
#include "ft_string.h"
#include "ft_ctype.h"

void	expand_argv_remove_quote(t_command cmd)
{
	int	i;

	if (cmd->type != CMD_SIMPLE)
		return ;
	i = -1;
	while (cmd->simple->cmd_argv[++i])
		expand_str_remove_quote(&cmd->simple->cmd_argv[i]);
}

void	expand_argv_split(t_command cmd)
{
	int			argv_i;
	t_arraylist	lst_new_args;
	char		**split;
	int			split_i;

	if (cmd->type != CMD_SIMPLE)
		return ;
	lst_new_args = ft_arraylist_new((t_consumer) free);
	argv_i = -1;
	while (cmd->simple->cmd_argv[++argv_i])
	{
		split = expand_split_str(cmd->simple->cmd_argv[argv_i], \
				(t_pred_int) ft_isspace);
		split_i = -1;
		while (split[++split_i])
			lst_new_args = ft_arraylist_add(lst_new_args, \
				ft_strdup(split[split_i]));
		ft_strarr_free(split);
	}
	ft_strarr_free(cmd->simple->cmd_argv);
	cmd->simple->cmd_argv = ft_lststr_to_arrstr(lst_new_args);
	ft_arraylist_destroy(lst_new_args);
	cmd->simple->cmd_argc = 0;
	while (cmd->simple->cmd_argv[cmd->simple->cmd_argc])
		cmd->simple->cmd_argc++;
}

void	expand_argv_dollar(t_command cmd, sig_atomic_t last_status_code)
{
	int	i;

	if (cmd->type != CMD_SIMPLE)
		return ;
	i = -1;
	while (cmd->simple->cmd_argv[++i])
		expand_str_dollar(&cmd->simple->cmd_argv[i], last_status_code);
}

void	expand_argv(t_command cmd, sig_atomic_t last_status_code)
{
	expand_argv_dollar(cmd, last_status_code);
	expand_argv_split(cmd);
	expand_argv_remove_quote(cmd);
}
