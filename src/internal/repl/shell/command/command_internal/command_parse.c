/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 04:51:10 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/28 05:38:00 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_stdio.h"
#include "ft_string.h"
#include "ft_util.h"
#include "internal/default.h"
#include "internal/env/env_internal/env_internal.h"

static char	**get_env_path(void);
static char	*build_bin_with_path(char *bin, char **env_path);

char	*parse_bin_path(char *bin)
{
	char	*bin_path;
	char	**env_path;

	if (access(bin, X_OK | F_OK | R_OK) == DEFAULT)
		return (ft_strdup(bin));
	if (*bin == '.' || *bin == '/' || *bin == '~')
		return (ft_strdup(bin));
	env_path = get_env_path();
	bin_path = build_bin_with_path(bin, env_path);
	if (env_path)
		ft_strarr_free(env_path);
	return (bin_path);
}

static char	**get_env_path(void)
{
	t_env_var	*env_vars;

	env_vars = env_parse_key("PATH");
	if (env_vars)
		return (ft_split(env_vars->value, ':'));
	else
		return (NULL);
}

static char	*build_bin_with_path(char *bin, char **env_path)
{
	char	*bin_path;

	while (*env_path)
	{
		ft_asprintf(&bin_path, "%s/%s", *env_path++, bin);
		if (!access(bin_path, X_OK | F_OK))
			return (bin_path);
		free(bin_path);
	}
	return (ft_strdup(bin));
}
