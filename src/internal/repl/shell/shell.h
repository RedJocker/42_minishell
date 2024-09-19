/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 06:32:34 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 08:36:38 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <stdbool.h>
# include "internal/repl/shell/command/command.h"
# include "internal/repl/shell/token/token.h"

typedef struct s_shell
{
	t_command	command;
	t_token		**tokens;
	bool		redisplay_status;
	int			status;
	int			tokens_len;
	char		**str_tokens;
	char		*input;
}	t_shell;

t_shell	*shell(void);
bool	shell_get_redisplay_status(void);
void	shell_set_input(void);
char	*shell_get_input(void);
void	shell_build_token(void);
void	shell_build_command(void);
void	shell_run_command(void);

#endif
