/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 00:09:21 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/18 20:34:58 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_H
# define ENVP_H

char	**get_envp(void);
char	*get_bin_path_with_envp(char *bin, char **envp);
char	*envp_find_bin_by_name(char *name, char **envp); // TODO: remove

#endif
