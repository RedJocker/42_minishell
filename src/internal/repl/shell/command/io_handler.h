/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handler.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 19:07:28 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/21 17:14:47 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_HANDLER_H
# define IO_HANDLER_H

# include "internal/repl/shell/command/command.h"

typedef enum e_io_handler_type
{
	IO_NONE,
	IO_FD,
	IO_PATH,
	IO_HEREDOC,
	IO_ERROR,
}	t_io_handler_type;

typedef enum e_io_direction
{
	IO_IN,
	IO_OUT,
}	t_io_direction;

typedef struct s_io_handler
{
	t_io_handler_type	type;
	t_io_direction		direction;
	union
	{
		int		fd;
		struct
		{
			char	*path;
			int		flags;
			int		mode;
		};
		char	*heredoc_limiter;
		struct
		{
			int		error_status;
			char	*error;
		};
	};
}	t_io_handler;

void	io_handler_set_path(
			t_io_handler *io,
			char *path,
			int flags_mode[2],
			t_io_direction io_dir);
void	io_handler_redirect(t_io_handler *io, char **out_errmsg);
int		io_handlers_redirect(t_arraylist lst_iohandlers, char **out_errmsg);
void	io_handler_destroy(t_io_handler *io);
void	io_handlers_add_path(
			t_arraylist *lst_iohandlers,
			char *path,
			int flags_mode[2],
			t_io_direction io_dir);
void	io_handler_to_fd(t_io_handler *io, char **out_errmsg);
int		io_handlers_to_fd(t_arraylist lst_iohandlers, char **out_errmsg);
void	io_handlers_add_pipe(
			t_arraylist *lst_iohandlers,
			int fd,
			t_io_direction io_dir);
void	io_handlers_close(t_arraylist lst_iohandlers);

#endif
