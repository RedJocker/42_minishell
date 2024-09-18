/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:12:04 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/18 04:53:37 by dande-je         ###   ########.fr       */
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
