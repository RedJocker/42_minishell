/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 02:29:58 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/14 03:18:21 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_INTERNAL_H
# define ENVIRON_INTERNAL_H

typedef struct s_environ_vars	t_environ_vars;
struct s_environ_vars
{
	t_environ_vars	*next;
	t_environ_vars	*prev;
	char	*key;
	char	*value;
};

typedef struct s_environ	t_environ;
struct s_environ
{
	t_environ_vars	*environ_vars;
	int				environ_size;
};

t_environ_vars	*environ_var_new(char *key, char *value);
t_environ_vars	*environ_var_last(t_environ_vars *environ_var);
void			environ_var_add_back(t_environ_vars **environ_var, \
					t_environ_vars *environ_var_new);
void			environ_destroy(t_environ_vars *environ_var);

#endif
