/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:45:47 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/06 12:46:15 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	destroy(t_philo *philo)
{
	free((*philo).r_fork);
	free(philo);
}

void	init_fork(t_philo *philo, int count)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * count);
	i = -1;
	while (++i < count)
		pthread_mutex_init(&forks[i], NULL);
	i = -1;
	while (++i < count)
	{
		philo[i].r_fork = &forks[i];
		if (i)
			philo[i].l_fork = &forks[i - 1];
		else if (!i && count > 1)
			philo[i].l_fork = &forks[count - 1];
		else
			philo[i].l_fork = NULL;
	}
}

void	init(t_philo **philo, t_time time, int count, int iteration)
{
	int	i;

	*philo = malloc(sizeof(t_philo) * count);
	i = -1;
	while (++i < count)
	{
		(*philo)[i].n = i;
		(*philo)[i].time.die = time.die;
		(*philo)[i].time.eat = time.eat;
		(*philo)[i].time.sleep = time.sleep;
		(*philo)[i].i = iteration;
		(*philo)[i].last_eat = timestamp();
	}
}
