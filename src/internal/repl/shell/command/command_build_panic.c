/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build_panic.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 00:48:32 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/14 01:05:12 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdio.h"
#include "internal/default.h"
#include "command_internal.h"

static const char	*g_err_template = "minishell: syntax error "\
							"near unexpected token `%s'";

t_command	command_build_panic_zero(char *unexpected)
{
	t_command	cmd_invalid;
	char		*err_msg;

	ft_asprintf(&err_msg, g_err_template, unexpected);
	cmd_invalid = command_invalid_new(err_msg, EXIT_SYNTAX_ERROR);
	free(err_msg);
	return (cmd_invalid);
}

t_command	command_build_panic_one(t_command cmd_to_free, char *unexpected)
{
	command_destroy(cmd_to_free);
	return (command_build_panic_zero(unexpected));
}

t_command	command_build_panic_two(
	t_command cmd_to_free1, t_command cmd_to_free2, char *unexpected)
{
	command_destroy(cmd_to_free1);
	return (command_build_panic_one(cmd_to_free2, unexpected));
}
