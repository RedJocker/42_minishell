/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 03:18:36 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/17 04:06:26 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal_bonus/default.h"
#include "internal_bonus/repl/shell/command/command.h"

static void	print_pwd_error(char *msg);

sig_atomic_t	runner_cmd_builtin_pwd(t_command cmd)
{
	char	*pwd;

	(void)cmd;
	pwd = getcwd(NULL, DEFAULT);
	if (!pwd)
	{
		if (errno == EACCES)
			print_pwd_error("permission denied");
		else if (errno == ENOMEM)
			print_pwd_error("cannot allocate memory");
		else if (errno == ENOENT)
			print_pwd_error("no such file or directory");
		else
			print_pwd_error("error getting current directory");
		return (errno % MAX_STATUS_CODE_SIZE);
	}
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", ft_strlen("\n"));
	free(pwd);
	return (EXIT_OK);
}

static void	print_pwd_error(char *msg)
{
	char	*error_msg;

	ft_asprintf(&error_msg, \
		"minishell: pwd: %s\n", msg);
	write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	free(error_msg);
}
