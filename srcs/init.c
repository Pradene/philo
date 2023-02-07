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

void	destroy(t_philo *philo, int count)
{
	int	i;

	i = -1;
	while (++i < count)
		free(philo[i].rf);
	free(philo);
}

void	init_fork(t_philo *philo, int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		philo[i].rf = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo[i].rf, NULL);
		philo[(i + 1) % count].lf = philo[i].rf;
	}
}

void	init(t_philo **philo, t_time time, int count, int iteration)
{
	int	i;

	i = -1;
	*philo = malloc(sizeof(t_philo) * count);
	init_fork(*philo, count);
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
