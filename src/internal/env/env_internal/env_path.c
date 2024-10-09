/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 04:21:54 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/09 03:31:53 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdio.h"
#include "ft_string.h"
#include "internal/env/env.h"
#include "internal/env/env_internal/env_internal.h"

char	**get_env_path(void)
{
	char	*path;
	char	**env_path;

	path = env_get_value("PATH");
	if (path)
	{
		env_path = (ft_split(path, ':'));
		free(path);
		return (env_path);
	}
	else
		return (NULL);
}

char	*build_bin_with_path(char *bin, char **env_path)
{
	char	*bin_path;

	while (env_path && *env_path)
	{
		ft_asprintf(&bin_path, "%s/%s", *env_path++, bin);
		if (!access(bin_path, X_OK | F_OK))
			return (bin_path);
		free(bin_path);
	}
	return (ft_strdup(bin));
}
