/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:10:47 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/16 02:42:23 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "internal/env/env_internal/env_internal.h"
# include <stdbool.h>

void	env_initializer(void);
void	env_destroy(void);
char	*env_parse(char *env_var, t_type_env_var type);
char	*env_get_bin(char *key);
char	*env_get_value(char *key);
int		env_get_key_len(char *env_var);
void	env_set_value(char *key, char *value);
bool	env_destroy_single_var(char *key);

#endif
