/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:38:29 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/16 04:13:07 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "ft_util.h"
#include "internal/repl/shell/runner/expand/expand_internal.h"

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
