/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 03:21:44 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/10 03:21:45 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	wait(t_philo *p, size_t time)
{
	size_t	i;

	i = 0;
	while (++i <= time / 10)
	{
		usleep(1 * 1000 * 10);
		pthread_mutex_lock(&p->prm->m_dead);
		if (p->prm->dead)
		{
			pthread_mutex_unlock(&p->prm->m_dead);
			return ;
		}
		pthread_mutex_unlock(&p->prm->m_dead);
	}
}

static void	lock_fork(t_philo *p, int n)
{
	if (p->id % 2)
	{
		if (n)
			pthread_mutex_lock(p->m_rf);
		else
			pthread_mutex_lock(p->m_lf);
	}
	else
	{
		if (n)
			pthread_mutex_lock(p->m_lf);
		else
			pthread_mutex_lock(p->m_rf);
	}
	print(p, FORK);
}

static int	eat(t_philo *p)
{
	lock_fork(p, 1);
	if (p->prm->count == 1)
	{
		pthread_mutex_unlock(p->m_rf);
		wait(p, p->prm->d_time);
		return (1);
	}
	lock_fork(p, 0);
	pthread_mutex_lock(&p->m_lasteat);
	p->last_eat = timestamp();
	pthread_mutex_unlock(&p->m_lasteat);
	print(p, EAT);
	p->eat++;
	wait(p, p->prm->e_time);
	pthread_mutex_unlock(p->m_lf);
	pthread_mutex_unlock(p->m_rf);
	if (p->eat == p->prm->rep)
	{
		pthread_mutex_lock(&p->prm->m_finished);
		p->prm->finished++;
		pthread_mutex_unlock(&p->prm->m_finished);
		return (1);
	}
	return (0);
}

void	wait_threads(t_prm *prm)
{
	pthread_mutex_lock(&prm->m_started);
	prm->started++;
	pthread_mutex_unlock(&prm->m_started);
	while (1)
	{
		pthread_mutex_lock(&prm->m_started);
		if (prm->started == prm->count)
		{
			pthread_mutex_unlock(&prm->m_started);
			return ;
		}
		pthread_mutex_unlock(&prm->m_started);
		usleep(100);
	}
}

void	*routine(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	wait_threads(p->prm);
	if ((p->id + 1) % 2)
		wait(p, p->prm->e_time / 2);
	while (1)
	{
		pthread_mutex_lock(&p->prm->m_dead);
		if (p->prm->dead)
		{
			pthread_mutex_unlock(&p->prm->m_dead);
			break ;
		}
		pthread_mutex_unlock(&p->prm->m_dead);
		if (eat(p))
			break ;
		print(p, SLEEP);
		wait(p, p->prm->s_time);
		print(p, THINK);
		if (p->prm->count % 2)
			wait(p, p->prm->e_time / 2);
	}
	return (NULL);
}
