/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 17:27:45 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/27 00:14:28 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_string.h"
#include "ft_util.h"
#include "internal/envp.h"

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

	sname_tname[1] = ft_strtrim(name, SPACE_CHARS);
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
