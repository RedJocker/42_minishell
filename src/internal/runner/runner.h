/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 01:39:11 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/31 19:35:25 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUNNER_H
# define RUNNER_H

# include "internal/command/command.h"

typedef	enum e_builtin
{
	NOT_BUILTIN,
	BUILTIN_ECHO
}	t_builtin;

int	runner(t_command cmd);

#endif
