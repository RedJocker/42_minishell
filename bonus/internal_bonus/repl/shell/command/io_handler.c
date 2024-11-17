/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 19:07:14 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:08:22 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "internal_bonus/repl/shell/command/io_handler.h"

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

void	io_handler_path_to_fd(t_io_handler *io)
{
	int		fd;
	char	*errmsg;

	fd = open(io->path, io->flags, io->mode);
	if (fd < 0)
	{
		ft_asprintf(&errmsg, "minishell: %s: %s", io->path, strerror(errno));
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
		ft_asprintf(&errmsg, "minishell: %s", strerror(errno));
		io_handler_set_error(io, errno, errmsg);
	}
	close(io->fd);
}
