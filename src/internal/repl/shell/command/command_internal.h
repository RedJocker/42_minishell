/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 19:15:47 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/05 02:23:04 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_INTERNAL_H
# define COMMAND_INTERNAL_H

# include "internal/repl/shell/command/command.h"
# include "internal/repl/shell/command/io_handler.h"

int			command_debug_id(void);
int			command_token_precedence(t_token_type token_type);
t_command	command_new(t_command_type type, char *type_str);
void		command_free(t_command cmd);
int			command_operator_idx(t_token **tokens, int tokens_len);
t_command	command_invalid_new(char *message, int status);
void		command_invalid_destroy(t_command cmd);
void		command_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir);
//
t_command	command_simple_new(
				t_token **tokens, int endtoken_idx);
void		command_simple_destroy(t_command cmd);
int			command_simple_is_invalid(
				t_token **tokens, int *endtoken_idx);
void		command_simple_add_pipe_io(
				t_command cmd, int pipe_fd, t_io_direction dir);
//
t_command	command_pipe_new(t_command cmd_before, t_command cmd_after);
void		command_pipe_destroy(t_command cmd);
void		command_pipe_add_pipe_io(
				t_command cmd, int pipe_fd, t_io_direction dir);
//
t_command	command_eof_new(void);
void		command_eof_destroy(t_command cmd);
//
t_command	command_and_new(t_command cmd_before, t_command cmd_after);
void        command_and_destroy(t_command cmd);
void		command_and_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir);
//
t_command	command_or_new(t_command cmd_before, t_command cmd_after);
void        command_or_destroy(t_command cmd);
void		command_or_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir);
//
t_command	command_paren_new(t_command cmd);
void        command_paren_destroy(t_command cmd);
void		command_paren_add_pipe_io(t_command cmd, int pipe_fd, t_io_direction dir);
#endif
