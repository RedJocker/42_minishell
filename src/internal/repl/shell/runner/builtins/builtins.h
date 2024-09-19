/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:56:01 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 08:40:00 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "internal/repl/shell/command/command.h"

int	runner_cmd_builtin_echo(t_command cmd);
int	runner_cmd_builtin_export(t_command cmd);

#endif
