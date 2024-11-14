/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 00:52:37 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/13 22:41:53 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

typedef enum e_token_type
{
	OP_REDIRECT_OUT_TRUNC,
	OP_REDIRECT_OUT_APPND,
	OP_REDIRECT_IN,
	OP_REDIRECT_IN_HEREDOC,
	OP_PIPE,
	OP_AND,
	OP_OR,
	OP_PAREN_OPEN,
	OP_PAREN_CLOSE,
	OP_NEWLINE,
	WORD,
	OP_EOF,
	INVALID,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
}	t_token;

t_token	**tokens_classify(char **arr_str_tokens, int *out_len);
void	tokens_destroy(t_token **tokens);
int		token_type_is_redirect(t_token *token);
int		token_type_is_word(t_token *token);

#endif
