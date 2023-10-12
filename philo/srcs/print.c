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
	pthread_mutex_lock(&p->prm->m_dead);
	if (p->prm->dead)
	{
		pthread_mutex_unlock(&p->prm->m_dead);
		return ;
	}
	pthread_mutex_unlock(&p->prm->m_dead);
	pthread_mutex_lock(&p->m_lasteat);
	pthread_mutex_lock(&p->prm->m_write);
	if (state == FORK)
		printf("%zu %d has taken a fork\n", timestamp(), p->id);
	else if (state == EAT)
		printf("%zu %d is eating\n", timestamp(), p->id);
	else if (state == SLEEP)
		printf("%zu %d is sleeping\n", timestamp(), p->id);
	else if (state == THINK)
		printf("%zu %d is thinking\n", timestamp(), p->id);
	pthread_mutex_unlock(&p->prm->m_write);
	pthread_mutex_unlock(&p->m_lasteat);
}
