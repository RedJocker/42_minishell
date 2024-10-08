/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:43:53 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/07 18:54:18 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "ft_memlib.h"
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
#include "io_handler.h"

void io_handler_set_heredoc(t_io_handler *io, char *heredoc_limit)
{
	io->type = IO_HEREDOC;
	io->direction = IO_IN;
	io->heredoc_limiter = ft_strdup(heredoc_limit);
}

void io_handlers_add_heredoc(t_arraylist *lst_ios, char *heredoc_limit)
{
	t_io_handler	*io;

	io = ft_calloc(1, sizeof(t_io_handler));
	io_handler_set_heredoc(io, heredoc_limit);
	*lst_ios = ft_arraylist_add(*lst_ios, io);
}

void io_handlers_heredoc_to_fd(t_arraylist ios)
{
	ft_arraylist_foreach(ios, (t_consumer) io_handler_heredoc_to_fd);
}

// tmp_fd[0]: read, tmp_fd[1]: write
void io_handler_heredoc_to_fd(t_io_handler *io)
{
	char	*line;
	int		delim_len;
	int		tmp_fd[2];
	char	endline;

	endline = '\n';
	//ft_printf("io_handler_heredoc %d\n", io->type);
	if (io->type != IO_HEREDOC)
		return;
	pipe(tmp_fd);
	delim_len = ft_strlen(io->heredoc_limiter);
	line = readline("> ");
	while (line && ft_strncmp(line, io->heredoc_limiter, delim_len) != 0)
	{
		//ft_printf("heredoc %s %s\n", io->heredoc_limiter, line);
		write(tmp_fd[1], line, ft_strlen(line));
		write(tmp_fd[1], &endline, 1);
		free(line);
		line = readline("> ");
	}
	close(tmp_fd[1]);
	io->type = IO_FD;
	io->fd = tmp_fd[0];
	if (line)
	{
		//ft_printf("finish heredoc with %s\n", line);
		free(line);
	}
	else
		ft_putendl("warning finish heredoc with end of file");
}
