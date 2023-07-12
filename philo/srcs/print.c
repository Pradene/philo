/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 03:18:56 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/10 03:18:58 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print(t_philo *p, t_state state)
{
	static int	i = 1;

	pthread_mutex_lock(&p->prm->m_write);
	pthread_mutex_lock(&p->prm->m_dead);
	if (p->prm->dead && i)
		printf("%ld %d died\n", timestamp(), p->id + --i);
	else if (state == FORK && !p->prm->dead && (p->prm->rep == -1
			|| p->eat < p->prm->rep))
		printf("%ld %d has taken a fork\n", timestamp(), p->id);
	else if (state == EAT && !p->prm->dead && (p->prm->rep == -1
			|| p->eat < p->prm->rep))
		printf("%ld %d is eating\n", timestamp(), p->id);
	else if (state == SLEEP && !p->prm->dead && (p->prm->rep == -1
			|| p->eat < p->prm->rep))
		printf("%ld %d is sleeping\n", timestamp(), p->id);
	else if (state == THINK && !p->prm->dead && (p->prm->rep == -1
			|| p->eat < p->prm->rep))
		printf("%ld %d is thinking\n", timestamp(), p->id);
	pthread_mutex_unlock(&p->prm->m_write);
	pthread_mutex_unlock(&p->prm->m_dead);
}
