/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 00:09:21 by dande-je          #+#    #+#             */
/*   Updated: 2024/10/15 05:43:14 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_H
# define ENVP_H

typedef enum e_envp_type	t_envp_type;
enum e_envp_type
{
	ENVP_DEFAULT,
	ENVP_EXPORT,
};

char	**get_envp(t_envp_type	type);

#endif
