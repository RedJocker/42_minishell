/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 01:11:38 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/14 04:04:58 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "collection/ft_arraylist.h"
# include "internal/repl/token/token.h"

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

typedef struct s_command_simple
{
	char	*cmd_path;
	int		cmd_argc;
	char	**cmd_argv;
	char	**cmd_envp; // TODO: Verify this var.
}	t_command_simple;

typedef struct s_command_invalid
{
	char	*msg;
}	t_command_invalid;

typedef struct s_command	*t_command;

typedef enum e_command_type
{
	CMD_SIMPLE,
	CMD_INVALID
}	t_command_type;

struct s_command
{
	char			*debug_id;
	t_command_type	type;
	t_arraylist		io_handlers;
	union
	{
		t_command_simple	*simple;
		t_command_invalid	*invalid;
	};
};

t_command	command_build(t_token **tokens, int tokens_len);
void		command_destroy(t_command cmd);

#endif
