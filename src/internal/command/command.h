/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 01:11:38 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/25 20:23:31 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "minishell.h"

t_command	command_build(t_token **tokens, int tokens_len);
void		command_destroy(t_command cmd);

#endif
