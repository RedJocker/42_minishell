/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:16 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/13 00:39:31 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPL_H
# define REPL_H

# include <stdbool.h>
# include "internal/repl/command/command.h"
# include "internal/token/token.h"

typedef struct s_repl
{
	t_command	command;
	t_token		**tokens;
	bool		redisplay_status;
	int			status;
	int			tokens_len;
	char		**str_tokens;
	char		*input;
}	t_repl;

int		repl(void);
t_repl	*mini(void);

#endif
