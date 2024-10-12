/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 19:07:14 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/11 02:05:57 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal/repl/shell/command/io_handler.h"

void	io_handler_destroy(t_io_handler *io)
{
	if (io->type == IO_PATH)
		free(io->path);
	else if (io->type == IO_HEREDOC)
	{
		free(io->heredoc_limiter);
		free(io->heredoc_input);
	}
	else if (io->type == IO_ERROR)
		free(io->error);
	free(io);
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

void	io_handler_path_to_fd(t_io_handler *io)
{
	int		fd;
	char	*errmsg;

	fd = open(io->path, io->flags, io->mode);
	if (fd < 0)
	{
		ft_asprintf(&errmsg, "bash: %s: %s", io->path, strerror(errno));
		free(io->path);
		io_handler_set_error(io, errno, errmsg);
		return ;
	}
	free(io->path);
	io->type = IO_FD;
	io->fd = fd;
}

void	io_handler_to_fd(t_io_handler *io)
{
	if (io->type == IO_PATH)
		io_handler_path_to_fd(io);
}

void	io_handler_redirect(t_io_handler *io)
{
	int		fd_target;
	char	*errmsg;

	fd_target = (int) io->direction;
	io_handler_to_fd(io);
	if (io->type != IO_FD)
		return ;
	if (dup2(io->fd, fd_target) < 0)
	{
		ft_asprintf(&errmsg, "bash: %s", strerror(errno));
		io_handler_set_error(io, errno, errmsg);
	}
	close(io->fd);
}
