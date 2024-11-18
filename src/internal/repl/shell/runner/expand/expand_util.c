/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:54:06 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/16 17:56:30 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <unistd.h>
#include "collection/ft_arraylist.h"
#include "ft_string.h"
#include "ft_util.h"
#include "expand_internal.h"

void	lst_files_init(t_arraylist	*lst_files)
{
	char			*cwd;
	DIR				*dp;
	struct dirent	*dirp;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	dp = opendir(cwd);
	free(cwd);
	*lst_files = ft_arraylist_new(free);
	dirp = readdir(dp);
	while (dirp)
	{
		if (dirp->d_name[0] != '.')
			*lst_files = ft_arraylist_add(*lst_files, \
			ft_strdup(dirp->d_name));
		dirp = readdir(dp);
	}
	closedir(dp);
	ft_arraylist_sort(*lst_files, (t_intbifun) expand_icompare_str);
}
