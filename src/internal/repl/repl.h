/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:15:16 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/07 04:40:13 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPL_H
# define REPL_H

# include "internal/command/command.h"
# include "internal/token/token.h"

typedef struct s_repl
{
	char		**str_tokens;
	t_token		**tokens;
	int			status;
	char		*input;
	int			tokens_len;
	int			should_close;
	t_command	command;
}	t_repl;

int	repl(void);

#endif
