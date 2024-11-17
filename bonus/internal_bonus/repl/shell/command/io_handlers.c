/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 16:59:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/17 04:08:04 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_assert.h"
#include "collection/ft_arraylist.h"
#include "internal_bonus/repl/shell/command/io_handler.h"

int	io_handlers_redirect(t_arraylist lst_iohandlers)
{
	int				len;
	int				i;
	t_io_handler	*io;

	len = ft_arraylist_len(lst_iohandlers);
	i = -1;
	while (++i < len)
	{
		io = ft_arraylist_get(lst_iohandlers, i);
		io_handler_redirect(io);
		if (io->type == IO_ERROR)
			return (0);
	}
	return (1);
}

char	*io_handlers_get_error(t_arraylist lst_iohandlers)
{
	t_io_handler	*io;
	int				len;
	int				i;

	len = ft_arraylist_len(lst_iohandlers);
	i = -1;
	while (++i < len)
	{
		io = ft_arraylist_get(lst_iohandlers, i);
		if (io->type == IO_ERROR)
			return (io->error);
	}
	ft_assert(0, "unexpected lack of error on io_handlers_get_error");
	return (NULL);
}

int	io_handlers_to_fd(t_arraylist lst_iohandlers)
{
	int				len;
	int				i;
	t_io_handler	*io;

	len = ft_arraylist_len(lst_iohandlers);
	i = -1;
	while (++i < len)
	{
		io = ft_arraylist_get(lst_iohandlers, i);
		io_handler_to_fd(io);
		if (io->type == IO_ERROR)
			return (0);
	}
	return (1);
}

void	io_handlers_close(t_arraylist lst_iohandlers)
{
	int				len;
	int				i;
	t_io_handler	*io;

	len = ft_arraylist_len(lst_iohandlers);
	i = -1;
	while (++i < len)
	{
		io = ft_arraylist_get(lst_iohandlers, i);
		if (io->type == IO_FD)
			close(io->fd);
	}
}
