/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 02:29:58 by dande-je          #+#    #+#             */
/*   Updated: 2024/09/16 04:38:08 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_INTERNAL_H
# define ENV_INTERNAL_H

# include "internal/default.h"

typedef struct s_env_var	t_env_var;
struct s_env_var
{
	t_env_var	*next;
	t_env_var	*prev;
	char		*key;
	char		*value;
};

typedef struct s_env		t_env;
struct s_env
{
	t_env_var	*env_var;
	int			env_size;
};

typedef enum e_type_env_var	t_type_env_var;
enum e_type_env_var
{
	KEY,
	VALUE,
};

t_env		*env(void);
t_env_var	*env_var_new(char *key, char *value);
t_env_var	*env_var_last(t_env_var *env_var);
void		env_var_add_back(t_env_var **env_var, t_env_var *env_var_new);
void		env_set_vars_size(t_operations type);
void		env_vars_destroy(t_env_var *env_var);
char		*env_parse(char *env_var, t_type_env_var type);

#endif
