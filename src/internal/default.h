/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 01:31:47 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/30 23:04:34 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFAULT_H
# define DEFAULT_H

enum e_default_values
{
	DEFAULT = 0,
	DEFAULT_INIT = -1,
	DEFAULT_BEGIN = 1,
	FAIL = -1,
	WAIT = 1,
	CHAR_BYTE = 1,
	NULL_BYTE = 1,
	CMD_NAME = 1,
	MAX_ARGS = 2,
	MAX_STATUS_CODE_SIZE = 256,
};

typedef enum e_operations	t_operations;
enum e_operations
{
	SET = 1,
	GET = -1,
	ADD = 2,
	SUB,
};

typedef enum e_exit_code
{
	EXIT_OK = 0,
	EXIT_REDIRECT_FAIL = 1,
	EXIT_SYNTAX_ERROR = 2,
	EXIT_OUT_OF_MEMORY = 42,
	EXIT_FORK_FAIL = 99,
	EXIT_TEMPORARY = 111,
	EXIT_COMMAND_NOT_EXECUTABLE = 126,
	EXIT_COMMAND_NOT_FOUND = 127,
	EXIT_SIGINT = 130,
}	t_exit_code;

#endif
