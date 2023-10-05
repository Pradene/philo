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

static void	wait(t_philo *philo, size_t time)
{
	size_t	i;

	i = 0;
	while (++i <= time / 10)
	{
		usleep(1 * 1000 * 10);
		if (timestamp() - philo->last_eat >= philo->prm->d_time)
		{
			pthread_mutex_lock(&philo->prm->m_dead);
			philo->prm->dead = true;
			pthread_mutex_unlock(&philo->prm->m_dead);
			break ;
		}
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
}

static void	eat(t_philo *p)
{
	lock_fork(p, 1);
	print(p, FORK);
	if (p->prm->count == 1)
	{
		pthread_mutex_unlock(p->m_rf);
		wait(p, p->prm->d_time * 2);
		return ;
	}
	lock_fork(p, 0);
	print(p, FORK);
	print(p, EAT);
	p->eat++;
	wait(p, p->prm->e_time);
	p->last_eat = timestamp();
	pthread_mutex_unlock(p->m_lf);
	pthread_mutex_unlock(p->m_rf);
}

void	*routine(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	while (1)
	{
		pthread_mutex_lock(&p->prm->m_dead);
		if (p->prm->dead)
		{
			pthread_mutex_unlock(&p->prm->m_dead);
			break ;
		}
		pthread_mutex_unlock(&p->prm->m_dead);
		eat(p);
		if (p->prm->rep != -1 && p->eat == p->prm->rep)
			break ;
		print(p, SLEEP);
		wait(p, p->prm->s_time);
		print(p, THINK);
	}
	return (NULL);
}
