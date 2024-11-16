/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler_set.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:15:13 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 23:10:54 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io_handler.h"
#include "ft_string.h"
#include "stringbuilder.h"

static void	set_heredoc_delim(t_io_handler *io, char *heredoc_limit)
{
	int				i;
	char			open_quote;
	t_stringbuilder	builder;

	io->heredoc_should_expand = true;
	i = -1;
	builder = stringbuilder_new();
	open_quote = 0;
	while (heredoc_limit[++i])
	{
		if (open_quote && heredoc_limit[i] == open_quote)
			open_quote = 0;
		else if (!open_quote && (heredoc_limit[i] == '\'' \
					|| heredoc_limit[i] == '\"'))
		{
			open_quote = heredoc_limit[i];
			io->heredoc_should_expand = false;
		}
		else
			stringbuilder_addchar(builder, heredoc_limit[i]);
	}
	io->heredoc_limiter = stringbuilder_build(builder);
}

void	io_handler_set_error(t_io_handler *io, int err_no, char *strerr)
{
	io->type = IO_ERROR;
	io->error_status = err_no;
	io->error = strerr;
}

void	io_handler_set_path(
		t_io_handler *io, char *path, int flags_mode[2], t_io_direction io_dir)
{
	io->type = IO_PATH;
	io->path = ft_strdup(path);
	io->flags = flags_mode[0];
	io->mode = flags_mode[1];
	io->direction = io_dir;
}

void	io_handler_set_heredoc(t_io_handler *io, char *heredoc_limit)
{
	io->type = IO_HEREDOC;
	io->direction = IO_IN;
	set_heredoc_delim(io, heredoc_limit);
}
