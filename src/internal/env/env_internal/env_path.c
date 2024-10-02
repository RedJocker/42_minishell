/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 04:21:54 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/02 04:48:27 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdio.h"
#include "ft_string.h"
#include "internal/env/env.h"
#include "internal/env/env_internal/env_internal.h"

char	**get_env_path(void)
{
	char	*path;

	path = env_get_value("PATH");
	if (path)
		return (ft_split(path, ':'));
	else
		return (NULL);
}

char	*build_bin_with_path(char *bin, char **env_path)
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
