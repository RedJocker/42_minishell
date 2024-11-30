/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 01:33:57 by dande-je          #+#    #+#             */
/*   Updated: 2024/11/30 10:48:39 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "ft_string.h"
#include "internal/default.h"
#include "internal/env/env.h"
#include "internal/env/env_internal/env_internal.h"

static void	env_add_default_value(char *key, char *value);
static void	env_update_default_value(t_env_var *env_var, char *key, \
				char *value);

void	env_update_default_values(void)
{
	t_env_var	*env_var;

	env_var = env_get_key("SHLVL", env()->env_var);
	if (!env_var)
		env_add_default_value("SHLVL", "1");
	else
		env_update_default_value(env_var, "SHLVL", NULL);
	env_var = env_get_key("PS1", env()->env_var);
	if (!env_var)
		env_add_default_value("PS1=", "RedWillShell$ ");
	else
		env_update_default_value(env_var, "PS1", "RedWillShell$ ");
	env_var = env_get_key("PATH", env()->env_var);
	if (!env_var)
		env_add_default_value("PATH=", \
			"/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin");
}

static void	env_add_default_value(char *key, char *value)
{
	env_var_add_back(&env()->env_var, \
		env_var_new(ft_strdup(key), ft_strdup(value)));
	env_set_vars_size(ADD);
}

static void	env_update_default_value(t_env_var *env_var, char *key, char *value)
{
	char	*value_update;

	if (ft_strncmp("SHLVL", key, ft_strlen(key)) == DEFAULT)
	{
		value_update = ft_itoa(ft_atoi(env_var->value) + CHAR_BYTE);
		env_set_value("SHLVL", value_update, true);
		free(value_update);
	}
	else
		env_set_value(key, value, true);
}
