/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handlers_add.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:22:27 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/13 21:24:18 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "ft_memlib.h"
#include "internal/repl/shell/command/io_handler.h"

void	io_handlers_add_path(
	t_arraylist *lst_iohandlers,
	char *path,
	int flags_mode[2],
	t_io_direction io_dir)
{
	t_io_handler	*io;

	io = ft_calloc(1, sizeof(t_io_handler));
	io_handler_set_path(io, path, flags_mode, io_dir);
	*lst_iohandlers = ft_arraylist_add(*lst_iohandlers, io);
}

void	io_handlers_add_pipe(
	t_arraylist *lst_iohandlers,
	int fd,
	t_io_direction io_dir)
{
	t_io_handler	*io;

	io = ft_calloc(1, sizeof(t_io_handler));
	io->type = IO_FD;
	io->direction = io_dir;
	io->fd = fd;
	*lst_iohandlers = ft_arraylist_addat(*lst_iohandlers, io, 0);
}
