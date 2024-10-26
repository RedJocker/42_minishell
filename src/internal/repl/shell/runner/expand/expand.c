/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:38:29 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/25 22:07:57 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_util.h"
#include "internal/repl/shell/runner/expand/expand_internal.h"
#include <signal.h>

void	expand_ios(t_command cmd, sig_atomic_t last_status_code)
{
	ft_arraylist_foreacharg(cmd->io_handlers, \
		(t_biconsumer) expand_io, &last_status_code);
}

void	expand(t_command cmd, sig_atomic_t last_status_code)
{
	expand_argv(cmd, last_status_code);
	expand_ios(cmd, last_status_code);
}

/* 		dolarExpansion */
/* 		wordSplitting */
/* 		starExpansion */
/* 		quoteRemoval */
