/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:10:47 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/14 03:50:00 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_H
# define ENVIRON_H

# include "internal/default.h"

void	environ_initializer(void);
char	**environ_status(char **value, t_operations type);

#endif
