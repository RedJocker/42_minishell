/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 19:07:14 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/30 21:32:14 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "command.h"
#include "ft_memlib.h"
#include "ft_string.h"
#include "ft_util.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

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

void	io_handlers_add_path(
		t_arraylist *lst_iohandlers, char *path, int flags, int mode)
{
	t_io_handler *io;

	io = ft_calloc(1, sizeof(t_io_handler));
	io_handler_set_path(io, path, flags, mode);
	*lst_iohandlers = ft_arraylist_add(*lst_iohandlers, io);
}

void	io_handler_path_to_fd(t_io_handler *io)
{
	int fd;

	fd = open(io->path, io->flags, io->mode);
	if (fd < 0)
	{
		io_handler_set_error(io, errno, strerror(errno));
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

void	io_handler_redirect(t_io_handler *io, int fd)
{
	io_handler_to_fd(io);
	if (io->type != IO_FD)
		return ;
	if (dup2(io->fd, fd) < 0)
		io_handler_set_error(io, errno, strerror(errno));
}

int	io_handlers_redirect(t_arraylist lst_iohandlers, int fd)
{
	int				len;
	int				i;
	t_io_handler	*io;

	len = ft_arraylist_len(lst_iohandlers);
	i = -1;
	while (++i < len)
	{
		io = ft_arraylist_get(lst_iohandlers, i);
		io_handler_redirect(io, fd);
		if (io->type == IO_ERROR)
			return (0);
	}
	return (1);
}
