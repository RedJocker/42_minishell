/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:12:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/18 20:33:07 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdio.h"
#include "ft_string.h"
#include "internal/env/env_internal/env_internal.h"

char	**get_envp(void)
{
	t_env_var	*env_vars;

	env_vars = env_parse_key("PATH");
	if (env_vars)
		return (ft_split(env_vars->value, ':'));
	else
		return (NULL);
}

// TODO: make **env and add no execve.
char	*get_bin_path_with_envp(char *bin, char **envp)
{
	char	*bin_path;

	if (access(bin, X_OK | F_OK | R_OK) == DEFAULT)
		return (bin);
	if (*bin == '.' || *bin == '/')
		return (bin);
	while (*envp)
	{
		ft_asprintf(&bin_path, "%s/%s", *envp++, bin);
		if (!access(bin_path, X_OK | F_OK))
			return (bin_path);
		free(bin_path);
	}
	return (NULL);
}

//TODO: remove below

#include <unistd.h>
#include "ft_string.h"
#include "ft_util.h"

static int	envp_is_path(char *maybe_path)
{
	return (ft_strncmp("PATH=", maybe_path, 5) == 0);
}

// path + 5 -> after PATH=
static char	**envp_path_arr(char *path)
{
	return (ft_split(path + 5, ':'));
}

static char	**envp_get_path_arr(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (envp_is_path(envp[i]))
			return (envp_path_arr(envp[i]));
	}
	return (0);
}

char	*envp_find_bin_by_name(char *name, char **envp)
{
	char	*bin;
	char	**path_arr;
	int		i;
	char	*sname_tname[2];

	sname_tname[1] = ft_strtrim(name, " \r\n\t\v\f");
	if (access(sname_tname[1], X_OK) == 0 || sname_tname[1][0] == '\0')
		return (sname_tname[1]);
	sname_tname[0] = ft_strjoin("/", sname_tname[1]);
	path_arr = envp_get_path_arr(envp);
	i = -1;
	while (path_arr[++i])
	{
		bin = ft_strjoin(path_arr[i], sname_tname[0]);
		if (access(bin, X_OK) == 0)
			break ;
		free(bin);
		bin = NULL;
	}
	ft_strarr_free(path_arr);
	free(sname_tname[0]);
	if (!bin)
		bin = ft_strdup(sname_tname[1]);
	free(sname_tname[1]);
	return (bin);
}
