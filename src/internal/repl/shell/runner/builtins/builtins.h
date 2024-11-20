/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:56:01 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/20 20:23:09 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <signal.h>
# include "internal/default.h"
# include "internal/repl/shell/command/command.h"
# include "internal/repl/shell/runner/runner_internal.h"

typedef enum e_builtin_id	t_builtin_id;
enum e_builtin_id
{
	NOT_BUILTIN,
	BUILTIN_ECHO,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT,
	BUILTIN_PWD,
	BUILTIN_CD,
	BUILTIN_END,
	BUILTIN_BEGIN = DEFAULT_BEGIN,
	BUILTIN_SIZE = BUILTIN_END,
};

enum e_fd_builtin
{
	FD_IN,
	FD_OUT,
	FD_SIZE,
};

typedef sig_atomic_t		(*t_cmd_builtin_func)(t_command);

typedef struct s_builtin	t_builtin;
struct s_builtin
{
	t_builtin_id		id;
	char				*invocation;
	t_cmd_builtin_func	fn;
};

typedef struct s_builtins	t_builtins;
struct s_builtins
{
	t_builtin	builtin[BUILTIN_SIZE];
};

sig_atomic_t	runner_cmd_builtin_echo(t_command cmd);
sig_atomic_t	runner_cmd_builtin_export(t_command cmd);
sig_atomic_t	runner_cmd_builtin_unset(t_command cmd);
sig_atomic_t	runner_cmd_builtin_env(t_command cmd);
sig_atomic_t	runner_cmd_builtin_exit(t_command cmd);
sig_atomic_t	runner_cmd_builtin_pwd(t_command cmd);
sig_atomic_t	runner_cmd_builtin_cd(t_command cmd);
t_builtin_id	check_builtin(t_command cmd);
sig_atomic_t	runner_cmd_builtin(t_builtin_id builtin, t_command cmd);
sig_atomic_t	runner_cmd_builtin_without_fork(t_builtin_id builtin, \
					t_runner_data *run_data);
void			print_builtin_exit(
					t_builtin_id builtin,
					t_runner_data *run_data,
					sig_atomic_t status);

#endif
