/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 06:32:34 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/02 05:00:23 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <signal.h>
# include "internal/repl/shell/command/command.h"

typedef struct s_shell
{
	t_command		command;
	t_token			**tokens;
	sig_atomic_t	status;
	int				tokens_len;
	char			**str_tokens;
	char			*input;
}	t_shell;

void	shell_set_input(t_shell *shell);
void	shell_command(t_shell *shell);

#endif
