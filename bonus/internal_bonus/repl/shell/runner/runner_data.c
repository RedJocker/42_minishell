/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 09:39:03 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/20 01:40:15 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_util.h"
#include "collection/ft_arraylist.h"
#include "internal_bonus/repl/shell/runner/runner_internal.h"

void	runner_data_init(t_runner_data *run_data, t_command cmd, \
			sig_atomic_t last_cmd_status)
{
	run_data->last_cmd_status = last_cmd_status;
	run_data->base_cmd = cmd;
	run_data->cmd = cmd;
	run_data->pids = ft_arraylist_new(free);
	run_data->pipes_to_close = ft_arraylist_new(free);
	run_data->backup_pids = ft_arraylist_new((t_consumer) ft_arraylist_destroy);
	run_data->is_main = 1;
}

void	runner_data_clean(t_runner_data *run_data)
{
	ft_arraylist_destroy(run_data->pids);
	ft_arraylist_destroy(run_data->pipes_to_close);
	ft_arraylist_destroy(run_data->backup_pids);
	command_destroy(run_data->base_cmd);
}
