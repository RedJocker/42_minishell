/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 06:32:34 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/23 10:13:03 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <signal.h>
# include <stdbool.h>
# include "internal/repl/shell/command/command.h"

typedef struct s_shell
{
	t_command		command;
	t_token			**tokens;
	bool			redisplay_status;
	sig_atomic_t	status;
	int				tokens_len;
	char			**str_tokens;
	char			*input;
}	t_shell;

t_shell	*shell(void);
void	shell_set_input(t_shell *shell);
void	shell_command(t_shell *shell);

#endif
