/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:46:38 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/06 12:47:39 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	main(int argc, char **argv)
{
	t_prm		prm;
	t_philo		*p;

	prm.dead = false;
	prm.started = 0;
	prm.finished = 0;
	if (init_mutex(&prm))
		return (1);
	if (init_prm(&prm, argc, argv))
		return (1);
	start_time(1);
	if (init(&p, &prm))
		return (1);
	if (launch(&prm, p))
		return (1);
	destroy(p);
	return (0);
}
