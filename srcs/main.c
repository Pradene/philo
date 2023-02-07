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

void	wait(t_philo *philo, size_t time)
{
	size_t	i;

	i = 0;
	while (++i < time)
	{
		usleep(1 * 1000);
		if (timestamp() - philo->last_eat > philo->time.die)
		{
			printf("%ld %d is died\n", timestamp(), philo->n);
			exit(EXIT_FAILURE);
		}
	}
}

void	*print(void *philo)
{
	int		i;
	t_philo	*p;

	i = 0;
	p = (t_philo *)philo;
	while (1)
	{
		pthread_mutex_lock(p->r_fork);
		pthread_mutex_lock(p->l_fork);
		printf("%ld %d has taken a fork\n", timestamp(), p->n);
		printf("%ld %d has taken a fork\n", timestamp(), p->n);
		printf("%ld %d is eating\n", timestamp(), p->n);
		p->last_eat = timestamp();
		wait(p, p->time.eat);
		pthread_mutex_unlock(p->r_fork);
		pthread_mutex_unlock(p->l_fork);
		if (p->i != -1 && p->i == ++i)
			break ;
		printf("%ld %d is sleeping\n", timestamp(), p->n);
		wait(p, p->time.sleep);
		printf("%ld %d is thinking\n", timestamp(), p->n);
	}
	return (NULL);
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
	int			i;
	size_t		count;
	t_time		time;
	t_philo		*philo;

	if (argc != 5 && argc != 6)
		exit(EXIT_FAILURE);
	count = (size_t)atoi(argv[1]);
	time.die = (size_t)atoi(argv[2]);
	time.eat = (size_t)atoi(argv[3]);
	time.sleep = (size_t)atoi(argv[4]);
	if (argc == 6)
		i = atoi(argv[5]);
	else
		i = -1;
	init(&philo, time, count, i);
	init_fork(philo, count);
	launch(philo, count);
	destroy(philo);
	return (0);
}
