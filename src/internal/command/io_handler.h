/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 19:07:28 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/29 21:10:23 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef IO_HANDLER_H
# define IO_HANDLER_H

# include "command.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

void	io_handler_set_path(t_command cmd, char *path, int flags, int mode);
void    io_handler_redirect(t_io_handler *io, int fd);

#endif
