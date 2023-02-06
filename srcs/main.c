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

size_t	timestamp(void)
{
	struct timeval	time;
	size_t			timestamp;

	gettimeofday(&time, NULL);
	timestamp = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (timestamp);
}

void	*print(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	while (1)
	{
		pthread_mutex_lock(p->r_fork);
		pthread_mutex_lock(p->l_fork);
		printf("%ld %d has taken a fork\n", timestamp(), p->n);
		printf("%ld %d has taken a fork\n", timestamp(), p->n);
		printf("%ld %d is eating\n", timestamp(), p->n);
		usleep(p->time.eat * 1000);
		pthread_mutex_unlock(p->r_fork);
		pthread_mutex_unlock(p->l_fork);
		printf("%ld %d is sleeping\n", timestamp(), p->n);
		usleep(p->time.sleep * 1000);
		printf("%ld %d is thinking\n", timestamp(), p->n);
	}
}

void	launch(t_philo *philo, int count)
{
	pthread_t	*t;
	int			i;

	t = malloc(sizeof(pthread_t) * count);
	i = -1;
	while (++i < count)
		pthread_create(&t[i], NULL, &print, &philo[i]);
	i = -1;
	while (++i < count)
		pthread_join(t[i], NULL);
	free(t);
}

int	main(int argc, char **argv)
{
	size_t		count;
	t_time		time;
	t_philo		*philo;

	if (argc != 5 && argc != 6)
		exit(EXIT_FAILURE);
	count = (size_t)atoi(argv[1]);
	time.die = (size_t)atoi(argv[2]);
	time.eat = (size_t)atoi(argv[3]);
	time.sleep = (size_t)atoi(argv[4]);
	init(&philo, time, count);
	init_fork(philo, count);
	launch(philo, count);
	destroy(philo);
	return (0);
}
