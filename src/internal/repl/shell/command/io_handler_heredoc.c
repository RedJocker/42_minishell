/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:43:53 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/23 03:33:38 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include "collection/ft_arraylist.h"
#include "ft_memlib.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "stringbuilder.h"
#include "internal/repl/shell/command/io_handler.h"
#include "internal/signal/signal.h"

static const char		*g_endl = "\n";

void	io_handlers_add_heredoc(t_arraylist *lst_ios, char *heredoc_limit)
{
	t_io_handler	*io;

	io = ft_calloc(1, sizeof(t_io_handler));
	io_handler_set_heredoc(io, heredoc_limit);
	*lst_ios = ft_arraylist_add(*lst_ios, io);
}

void	io_handler_heredoc_clear(
	t_io_handler *io, char *line, t_stringbuilder	builder)
{
	free(line);
	stringbuilder_destroy(builder);
	free(io->heredoc_limiter);
	io_handler_set_error(io, 1, ft_strdup(""));
}

void	io_handler_heredoc_prompt(t_io_handler *io)
{
	char			*line;
	int				delim_len;
	t_stringbuilder	builder;

	if (io->type != IO_HEREDOC || signal_status(DEFAULT, GET) == SIGINT)
		return ;
	delim_len = ft_strlen(io->heredoc_limiter);
	builder = stringbuilder_new();
	line = readline("> ");
	if (signal_status(DEFAULT, GET) == SIGINT)
		return (io_handler_heredoc_clear(io, line, builder));
	while (line && ft_strncmp(line, io->heredoc_limiter, delim_len + 1) != 0 \
			&& signal_status(DEFAULT, GET) != SIGINT)
	{
		stringbuilder_addstr(&builder, line);
		stringbuilder_addstr(&builder, (char *) g_endl);
		free(line);
		line = readline("> ");
	}
	if (line)
		free(line);
	else
		ft_printf("minishell: warning: here-document delimited by " \
		"end-of-file (wanted `%s')\n", io->heredoc_limiter);
	io->heredoc_input = stringbuilder_build(builder);
}

void	io_handler_heredoc_to_fd(t_io_handler *io)
{
	int		tmp_fd[2];

	if (io->type != IO_HEREDOC)
		return ;
	pipe(tmp_fd);
	write(tmp_fd[1], io->heredoc_input, ft_strlen(io->heredoc_input));
	close(tmp_fd[1]);
	free(io->heredoc_limiter);
	free(io->heredoc_input);
	io->type = IO_FD;
	io->fd = tmp_fd[0];
}
