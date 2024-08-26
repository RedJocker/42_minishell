/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 00:51:25 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/25 22:17:14 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_token_type
{
	OP_REDIRECT_OUT_TRUNC,
	OP_REDIRECT_OUT_APPND,
	OP_REDIRECT_IN,
	OP_REDIRECT_IN_HEREDOC,
	OP_PIPE,
	WORD,
	INVALID,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
}	t_token;

typedef enum e_io_handler_type
{
	IO_NONE,
	IO_FD,
	IO_PATH,
	IO_HEREDOC,
	IO_ERROR,
}	t_io_handler_type;

typedef struct s_io_handler
{
	t_io_handler_type	type;
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
	char	**cmd_envp;
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
	t_io_handler	output;
	union
	{
		t_command_simple	*simple;
		t_command_invalid	*invalid;
	};
};

#endif
