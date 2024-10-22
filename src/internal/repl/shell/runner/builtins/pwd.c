/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 03:18:36 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/22 03:41:58 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include "ft_string.h"
#include "internal/default.h"
#include "internal/repl/shell/command/command.h"

sig_atomic_t	runner_cmd_builtin_pwd(t_command cmd)
{
	char	*pwd;
	char	buf[PATH_MAX];

	(void)cmd;
	pwd = getcwd(buf, PATH_MAX);
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", ft_strlen("\n"));
	return (EXIT_OK);
}
