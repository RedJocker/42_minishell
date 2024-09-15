/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 02:29:58 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/15 03:51:09 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_INTERNAL_H
# define ENVIRON_INTERNAL_H

# include "internal/default.h"

typedef struct s_environ_var	t_environ_var;
struct s_environ_var
{
	t_environ_var	*next;
	t_environ_var	*prev;
	char	*key;
	char	*value;
};

typedef struct s_environ	t_environ;
struct s_environ
{
	t_environ_var	*environ_var;
	int				environ_size;
};

t_environ		*environ(void);
t_environ_var	*environ_var_new(char *key, char *value);
t_environ_var	*environ_var_last(t_environ_var *environ_var);
void			environ_var_add_back(t_environ_var **environ_var, \
					t_environ_var *environ_var_new);
void			environ_set_vars_size(t_operations type);
void			environ_vars_destroy(t_environ_var *environ_var);
void			environ_parse(char *environ_var);

#endif
