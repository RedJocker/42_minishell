/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:10:47 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/02 04:39:57 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

void	env_initializer(void);
void	env_destroy(void);
char	*env_get_bin(char *key);
char	*env_get_value(char *key);
void	env_set_value(char *key, char *value);

#endif
