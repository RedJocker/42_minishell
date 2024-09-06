/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 19:07:14 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/05 01:59:13 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "command.h"
#include "ft_assert.h"
#include "ft_memlib.h"
#include "ft_string.h"
#include "ft_util.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "ft_stdio.h"

void	io_handler_destroy(t_io_handler *io)
{
	if (io->type == IO_PATH)
		free(io->path);
	else if (io->type == IO_HEREDOC)
		free(io->heredoc_limiter);
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

void	io_handler_set_path(t_io_handler *io, char *path, int flags, int mode)
{
	io->type = IO_PATH;
	io->path = ft_strdup(path);
	io->flags = flags;
	io->mode = mode;
}

void	io_handler_path_to_fd(t_io_handler *io, char **out_errmsg)
{
	int		fd;

	fd = open(io->path, io->flags, io->mode);
	if (fd < 0)
	{
		ft_asprintf(out_errmsg, "bash: %s: %s", io->path, strerror(errno));
		io_handler_set_error(io, errno, *out_errmsg);
		return ;
	}
	free(io->path);
	io->type = IO_FD;
	io->fd = fd;
}

void	io_handler_to_fd(t_io_handler *io, char **out_errmsg)
{
	if (io->type == IO_PATH)
		io_handler_path_to_fd(io, out_errmsg);
}

void	io_handler_redirect(t_io_handler *io, int fd, char **out_errmsg)
{

	io_handler_to_fd(io, out_errmsg);
	if (io->type != IO_FD)
		return ;
	if (dup2(io->fd, fd) < 0)
	{
		ft_asprintf(out_errmsg, "bash: %s", strerror(errno));
		io_handler_set_error(io, errno, *out_errmsg);
	}
}
