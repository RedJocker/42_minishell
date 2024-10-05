/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:56:01 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/01 02:04:34 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "internal/repl/shell/command/command.h"
# include "internal/repl/shell/runner/runner.h"

sig_atomic_t	runner_cmd_builtin_echo(t_command cmd);
sig_atomic_t	runner_cmd_builtin_export(t_command cmd);
sig_atomic_t	runner_cmd_builtin_nofork(t_builtin builtin, t_command cmd);

#endif
