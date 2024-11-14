/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:43:53 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/13 21:19:44 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "ft_memlib.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"
#include "internal/repl/shell/command/io_handler.h"
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "ft_string.h"
#include "ft_util.h"
#include "internal/signal/signal.h"
#include "io_handler.h"
#include "stringbuilder.h"

void	io_handlers_add_heredoc(t_arraylist *lst_ios, char *heredoc_limit)
{
	t_io_handler	*io;

	io = ft_calloc(1, sizeof(t_io_handler));
	io_handler_set_heredoc(io, heredoc_limit);
	*lst_ios = ft_arraylist_add(*lst_ios, io);
}

// tmp_fd[0]: read, tmp_fd[1]: write
void	io_handler_heredoc_prompt(t_io_handler *io)
{
	char			*line;
	int				delim_len;
	t_stringbuilder	builder;
	const char		*endline = "\n";

	if (io->type != IO_HEREDOC)
		return ;
	delim_len = ft_strlen(io->heredoc_limiter);
	builder = stringbuilder_new();
	line = readline("> ");
	if (signal_status(DEFAULT, GET) == SIGINT)
	{
		free(line);
		stringbuilder_destroy(builder);
		free(io->heredoc_limiter);
		io_handler_set_error(io, 1, ft_strdup(""));
		return ;
	}
	while (line && ft_strncmp(line, io->heredoc_limiter, delim_len + 1) != 0 \
			&& signal_status(DEFAULT, GET) != SIGINT)
	{
		stringbuilder_addstr(&builder, line);
		stringbuilder_addstr(&builder, (char *) endline);
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

void	io_handlers_heredoc_prompt(t_arraylist ios)
{
	ft_arraylist_foreach(ios, (t_consumer) io_handler_heredoc_prompt);
}

// tmp_fd[0]: read, tmp_fd[1]: write
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

void	io_handlers_heredoc_to_fd(t_arraylist ios)
{
	ft_arraylist_foreach(ios, (t_consumer) io_handler_heredoc_to_fd);
}
