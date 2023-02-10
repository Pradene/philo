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
		if (timestamp() - philo->last_eat >= philo->param->d_time)
		{
			pthread_mutex_lock(&philo->param->m_dead);
			philo->param->dead = true;
			pthread_mutex_unlock(&philo->param->m_dead);
			break ;
		}
	}
}

static void	eat(t_philo *p)
{
	pthread_mutex_lock(p->m_rf);
	print(p, FORK);
	if (p->param->count == 1)
	{
		pthread_mutex_unlock(p->m_rf);
		wait(p, p->param->d_time * 2);
		return ;
	}
	pthread_mutex_lock(p->m_lf);
	print(p, FORK);
	print(p, EAT);
	p->last_eat = timestamp();
	wait(p, p->param->e_time);
	pthread_mutex_lock(&p->param->m_eat);
	p->param->eat_count++;
	pthread_mutex_unlock(&p->param->m_eat);
	pthread_mutex_unlock(p->m_rf);
	pthread_mutex_unlock(p->m_lf);
}

void	*routine(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	if ((p->id + 1) % 2)
		wait(p, p->param->e_time / 2);
	while (1)
	{
		if (p->param->dead)
			break ;
		eat(p);
		if (p->param->rep != -1
			&& p->param->eat_count / p->param->count == p->param->rep)
			break ;
		print(p, SLEEP);
		wait(p, p->param->s_time);
		print(p, THINK);
	}
	return (NULL);
}