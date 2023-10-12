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

void	destroy(t_philo *philo)
{
	int	i;

	pthread_mutex_destroy(&philo->prm->m_write);
	pthread_mutex_destroy(&philo->prm->m_finished);
	pthread_mutex_destroy(&philo->prm->m_started);
	pthread_mutex_destroy(&philo->prm->m_dead);
	i = -1;
	while (++i < philo->prm->count)
	{
		pthread_mutex_destroy(philo[i].m_rf);
		pthread_mutex_destroy(&philo[i].m_lasteat);
	}
	i = -1;
	while (++i < philo->prm->count)
		free(philo[i].m_rf);
	free(philo);
}

int	check_death(t_prm *prm, t_philo *p)
{
	pthread_mutex_lock(&p->m_lasteat);
	if (timestamp() - p->last_eat >= prm->d_time)
	{
		pthread_mutex_unlock(&p->m_lasteat);
		pthread_mutex_lock(&prm->m_write);
		printf("%zu %d died\n", timestamp(), p->id);
		pthread_mutex_unlock(&prm->m_write);
		pthread_mutex_lock(&prm->m_dead);
		prm->dead = true;
		pthread_mutex_unlock(&prm->m_dead);
		return (1);
	}
	pthread_mutex_unlock(&p->m_lasteat);
	return (0);
}

void	monitoring(t_prm *prm, t_philo *p)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < prm->count)
		{
			pthread_mutex_lock(&prm->m_finished);
			if (prm->finished == prm->count)
			{
				pthread_mutex_unlock(&prm->m_finished);
				return ;
			}
			pthread_mutex_unlock(&prm->m_finished);
			if (check_death(prm, &p[i]))
				return ;
			usleep(100);
		}
	}
}

void	pthread_quit(pthread_t *t, int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		if (pthread_join(t[i], NULL))
			return ;
	}
}

int	launch(t_prm *prm, t_philo *p)
{
	pthread_t	*t;
	int			i;

	t = malloc(sizeof(pthread_t) * prm->count);
	if (!t)
		return (1);
	i = -1;
	pthread_mutex_lock(&prm->m_started);
	while (++i < prm->count)
	{
		if (pthread_create(&t[i], NULL, &routine, &p[i]))
		{
			pthread_mutex_unlock(&prm->m_started);
			pthread_quit(t, i);
			return (destroy(p), free(t), 1);
		}
		prm->started++;
	}
	pthread_mutex_unlock(&prm->m_started);
	monitoring(prm, p);
	pthread_quit(t, prm->count);
	free(t);
	return (0);
}

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
