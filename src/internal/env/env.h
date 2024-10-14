/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:10:47 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/14 03:18:55 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

void	env_initializer(void);
void	env_destroy(void);
char	*env_get_bin(char *key);
char	*env_get_value(char *key);
int		env_get_key_len(char *env_var);
void	env_set_value(char *key, char *value);

#endif
