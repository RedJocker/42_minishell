/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handlers_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 01:57:56 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/15 02:03:23 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection/ft_arraylist.h"
#include "internal/repl/shell/command/command.h"
#include "ft_util.h"
#include "internal/signal/signal.h"
#include "io_handler.h"

void	io_handlers_heredoc_to_fd(t_arraylist ios)
{
	ft_arraylist_foreach(ios, (t_consumer) io_handler_heredoc_to_fd);
}

void	io_handlers_heredoc_prompt(t_arraylist ios)
{
	ft_arraylist_foreach(ios, (t_consumer) io_handler_heredoc_prompt);
}
