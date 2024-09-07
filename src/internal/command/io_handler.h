/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 19:07:28 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/07 04:42:46 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef IO_HANDLER_H
# define IO_HANDLER_H

# include "internal/command/command.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

void	io_handler_set_path(t_io_handler *io, char *path, int flags, int mode);
void	io_handler_redirect(t_io_handler *io, int fd, char **out_errmsg);
int     io_handlers_redirect(
			t_arraylist lst_iohandlers, int fd, char **out_errmsg);
void	io_handler_destroy(t_io_handler *io);
void	io_handlers_add_path(
			t_arraylist *lst_iohandlers, char *path, int flags, int mode);
void	io_handler_to_fd(t_io_handler *io, char **out_errmsg);
int		io_handlers_to_fd(t_arraylist lst_iohandlers, char **out_errmsg);

#endif
