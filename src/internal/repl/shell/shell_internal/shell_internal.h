/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:15:17 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/21 17:28:24 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_INTERNAL_H
# define SHELL_INTERNAL_H

# include <stdbool.h>
# include "internal/repl/shell/command/command.h"
# include "internal/repl/shell/token/token.h"
# include "internal/repl/shell/shell.h"

void	shell_build_token(t_shell *shell);
void	shell_build_command(t_shell *shell);
void	shell_run(t_shell *shell);

#endif
