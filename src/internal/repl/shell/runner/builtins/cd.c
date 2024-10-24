/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 02:17:40 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/24 06:02:07 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "ft_string.h"
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/repl/shell/command/command.h"

static char			*get_pwd(void);
static sig_atomic_t	print_cd_error(char *msg, sig_atomic_t status);
static sig_atomic_t	cd_change_folder(char *path, sig_atomic_t status);
static void			update_value(char *key);

sig_atomic_t	runner_cmd_builtin_cd(t_command cmd)
{
	char	*path;

	path = NULL;
	if (cmd->simple->cmd_argc > MAX_ARGS)
		return (print_cd_error("too many arguments", EXIT_FAILURE));
	else if (!cmd->simple->cmd_argv[DEFAULT_BEGIN] \
		|| ft_strncmp(cmd->simple->cmd_argv[DEFAULT_BEGIN], "~", \
		CHAR_BYTE + NULL_BYTE) == DEFAULT)
	{
		path = env_get_value("HOME");
		if (!path)
			return (print_cd_error("HOME not set", EXIT_FAILURE));
	}
	else
		path = ft_strdup(cmd->simple->cmd_argv[DEFAULT_BEGIN]);
	return (cd_change_folder(path, EXIT_OK));
}

static sig_atomic_t	cd_change_folder(char *path, sig_atomic_t status)
{
	if (access(path, F_OK | R_OK) == DEFAULT)
		update_value("OLDPWD");
	if (access(path, F_OK))
		status = print_cd_error("no such file or directory", EXIT_FAILURE);
	else if (access(path, R_OK))
		status = print_cd_error("permission denied", EXIT_FAILURE);
	else
		chdir(path);
	if (access(path, F_OK | R_OK) == DEFAULT)
		update_value("PWD");
	if (path)
		free(path);
	return (status);
}

static void	update_value(char *key)
{
	char	*pwd;
	char	*env_var;

	pwd = get_pwd();
	env_var = env_get_value(key);
	if (env_var)
	{
		env_set_value(key, pwd, true);
		free(env_var);
	}
	free(pwd);
}

static char	*get_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, DEFAULT);
	if (!pwd)
		return (NULL);
	return (pwd);
}

static sig_atomic_t	print_cd_error(char *msg, sig_atomic_t status)
{
	char	*error_msg;

	ft_asprintf(&error_msg, \
		"bash: cd: %s\n", msg); // TODO: change the name of program to minishell instead of bash.
	write(STDOUT_FILENO, error_msg, ft_strlen(error_msg));
	free(error_msg);
	return (status);
}
