/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 21:10:58 by maurodri          #+#    #+#             */
/*   Updated: 2024/10/25 21:46:45 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_INTERNAL_H
# define EXPAND_INTERNAL_H

# include "stringbuilder.h"
# include "collection/ft_arraylist.h"
# include "internal/ft_extension.h"
# include "internal/repl/shell/command/command.h"
# include "internal/repl/shell/command/io_handler.h"
# include <signal.h>

char	**expand_split_str(char const *str, t_pred_int should_split);
void	expand_argv(t_command cmd, sig_atomic_t last_status_code);
void	expand_io(t_io_handler *io, sig_atomic_t *last_status_code);
void	expand_str_dollar(char **strptr, sig_atomic_t last_status_code);
void	expand_str_remove_quote(char **strptr);

#endif
