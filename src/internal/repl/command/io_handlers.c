/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 16:59:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/13 00:49:09 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memlib.h"
#include "internal/repl/command/command.h"
#include "internal/repl/command/io_handler.h"

int	io_handlers_redirect(t_arraylist lst_iohandlers, char **out_errmsg)
{
	int				len;
	int				i;
	t_io_handler	*io;

	len = ft_arraylist_len(lst_iohandlers);
	i = -1;
	while (++i < len)
	{
		io = ft_arraylist_get(lst_iohandlers, i);
		io_handler_redirect(io, out_errmsg);
		if (io->type == IO_ERROR)
			return (0);
	}
	return (1);
}

int	io_handlers_to_fd(t_arraylist lst_iohandlers, char **out_errmsg)
{
	int				len;
	int				i;
	t_io_handler	*io;

	len = ft_arraylist_len(lst_iohandlers);
	i = -1;
	while (++i < len)
	{
		io = ft_arraylist_get(lst_iohandlers, i);
		io_handler_to_fd(io, out_errmsg);
		if (io->type == IO_ERROR)
			return (0);
	}
	return (1);
}

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