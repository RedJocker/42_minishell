/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 00:51:25 by maurodri          #+#    #+#             */
/*   Updated: 2024/08/24 02:06:47 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_token_type
{
	OP_REDIRECT_OUT_TRUNC,
	OP_REDIRECT_OUT_APPND,
	OP_REDIRECT_IN,
	OP_REDIRECT_IN_HEREDOC,
	OP_PIPE,
	WORD,
	INVALID,
}	t_token_type;


typedef struct s_token
{
	t_token_type	type;
	char			*content;
}	t_token;

#endif
