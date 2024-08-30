/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 04:20:53 by dande-je          #+#    #+#             */
/*   Updated: 2024/08/30 04:26:19 by dande-je         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdbool.h>
#include <termios.h>

void	terminal_properties(bool reset)
{
	static struct termios	terminal;

	if (reset)
		tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
	else
		tcgetattr(STDIN_FILENO, &terminal);
}
