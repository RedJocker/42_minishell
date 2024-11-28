/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_io.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:41:12 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/27 23:49:09 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include "ft_ctype.h"
#include "ft_string.h"
#include "ft_stdio.h"
#include "ft_util.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/repl/shell/runner/expand/expand_internal.h"

void	expand_io_dollar(t_io_handler *io, sig_atomic_t last_status_code)
{
	if (io->type == IO_PATH)
		return (expand_str_dollar(&io->path, last_status_code));
	else if (io->type == IO_HEREDOC)
	{
		if (!io->heredoc_should_expand)
			return ;
		return (expand_str_dollar(&io->heredoc_input, last_status_code));
	}
}

void	expand_io_split(t_io_handler *io, char *original_path)
{
	char	**split;
	char	*errmsg;

	if (io->type != IO_PATH)
		return ;
	split = expand_split_str(io->path, (t_pred_int) ft_isspace);
	if (split[0] == NULL || split[1] != NULL)
	{
		ft_asprintf(&errmsg, \
			"minishell: %s: ambiguous redirect", original_path);
		free(io->path);
		ft_strarr_free(split);
		return (io_handler_set_error(io, errno, errmsg));
	}
	free(io->path);
	io->path = ft_strdup(split[0]);
	ft_strarr_free(split);
}

void	expand_io_star(t_io_handler *io, char *original_path)
{
	char		**res;
	char		*errmsg;
	t_arraylist	lst_new_args;

	if (io->type != IO_PATH)
		return ;
	lst_new_args = ft_arraylist_new(free);
	expand_str_star(io->path, &lst_new_args);
	res = ft_lststr_to_arrstr(lst_new_args);
	ft_arraylist_destroy(lst_new_args);
	if (res[0] == NULL || res[1] != NULL)
	{
		ft_asprintf(&errmsg, "minishell: %s: ambiguous redirect", \
			original_path);
		free(io->path);
		ft_strarr_free(res);
		return (io_handler_set_error(io, errno, errmsg));
	}
	free(io->path);
	io->path = ft_strdup(res[0]);
	ft_strarr_free(res);
}

void	expand_io_remove_quote(t_io_handler *io)
{
	if (io->type == IO_PATH)
		return (expand_str_remove_quote(&io->path));
}

void	expand_io(t_io_handler *io, sig_atomic_t *last_status_code)
{
	char	*original_path;

	if (io->type == IO_PATH)
		original_path = ft_strdup(io->path);
	else
		original_path = NULL;
	expand_io_dollar(io, *last_status_code);
	expand_io_split(io, original_path);
	expand_io_star(io, original_path);
	expand_io_remove_quote(io);
	if (original_path)
		free(original_path);
}
