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
	while (++i <= time)
	{
		usleep(1 * 1000);
		if (timestamp() - philo->last_eat > philo->param->d_time)
		{
			pthread_mutex_lock(&philo->param->m_dead);
			philo->param->dead = true;
			pthread_mutex_unlock(&philo->param->m_dead);
		}
	}
}

void	print(t_philo *philo, t_state state)
{
	static int	i = 1;

	pthread_mutex_lock(&philo->param->m_write);
	if (philo->param->dead && i)
		printf("%ld %d died\n", timestamp(), philo->id + --i);
	else if (state == FORK && !philo->param->dead)
		printf("%ld %d has taken a fork\n", timestamp(), philo->id);
	else if (state == EAT && !philo->param->dead)
		printf("%ld %d is eating\n", timestamp(), philo->id);
	else if (state == SLEEP && !philo->param->dead)
		printf("%ld %d is sleeping\n", timestamp(), philo->id);
	else if (state == THINK && !philo->param->dead)
		printf("%ld %d is thinking\n", timestamp(), philo->id);
	pthread_mutex_unlock(&philo->param->m_write);
}

void	*routine(void *philo)
{
	int		i;
	t_philo	*p;

	i = 0;
	p = (t_philo *)philo;
	if (p->id % 2)
		wait(p, p->param->e_time);
	while (1)
	{
		if (p->param->dead)
			break ;
		pthread_mutex_lock(p->m_rf);
		print(p, FORK);
		pthread_mutex_lock(p->m_lf);
		print(p, FORK);
		print(p, EAT);
		p->last_eat = timestamp();
		wait(p, p->param->e_time);
		pthread_mutex_unlock(p->m_rf);
		pthread_mutex_unlock(p->m_lf);
		if (p->param->rep != -1 && ++i == p->param->rep)
			break ;
		print(p, SLEEP);
		wait(p, p->param->s_time);
		print(p, THINK);
	}
	return (NULL);
}

void	launch(t_philo *philo)
{
	pthread_t	*t;
	int			i;
	int			count;

	count = philo->param->count;
	t = malloc(sizeof(pthread_t) * count);
	i = -1;
	while (++i < count)
		pthread_create(&t[i], NULL, &routine, &philo[i]);
	i = -1;
	while (++i < count)
		pthread_join(t[i], NULL);
	free(t);
}

int	main(int argc, char **argv)
{
	t_param		param;
	t_philo		*philo;

	init_param(&param, argc, argv);
	init(&philo, &param);
	launch(philo);
	destroy(philo);
	return (0);
}
