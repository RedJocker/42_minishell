/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 01:11:38 by maurodri          #+#    #+#             */
/*   Updated: 2024/09/25 00:41:36 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "collection/ft_arraylist.h"
# include "internal/repl/shell/token/token.h"

typedef struct s_command	*t_command;

typedef struct s_command_simple
{
	char	*cmd_path;
	int		cmd_argc;
	char	**cmd_argv;
	char	**cmd_envp;
}	t_command_simple;

typedef struct s_command_pipe
{
	t_command	cmd_before;
	t_command	cmd_after;
}	t_command_pipe;

typedef struct s_command_invalid
{
	char	*msg;
}	t_command_invalid;

typedef enum e_command_type
{
	CMD_SIMPLE,
	CMD_INVALID,
	CMD_PIPE,
	CMD_EOF,
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
		t_command_pipe		*pipe;
	};
};

t_command	command_build(t_token **tokens, int tokens_len);
void		command_destroy(t_command cmd);

#endif
