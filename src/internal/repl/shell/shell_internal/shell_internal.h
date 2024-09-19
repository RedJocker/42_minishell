/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:15:17 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/19 09:28:38 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_INTERNAL_H
# define SHELL_INTERNAL_H

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
void	shell_build_token(void);
void	shell_build_command(void);
void	shell_build_runner(void);

#endif
