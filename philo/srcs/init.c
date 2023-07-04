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

void	quit(t_philo *philo)
{
	pthread_mutex_destroy(&philo->prm->m_write);
	pthread_mutex_destroy(&philo->prm->m_dead);
	pthread_mutex_destroy(&philo->prm->m_eat);
}

void	destroy(t_philo *philo)
{
	int	i;

	pthread_mutex_destroy(&philo->prm->m_write);
	pthread_mutex_destroy(&philo->prm->m_dead);
	pthread_mutex_destroy(&philo->prm->m_eat);
	i = -1;
	while (++i < philo->prm->count)
		pthread_mutex_destroy(philo[i].m_rf);
	i = -1;
	while (++i < philo->prm->count)
		free(philo[i].m_rf);
	free(philo);
}

static void	init_prm(t_prm *prm, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		error("Invalid arguments : ./philo N_PHILO TIME_TO_DIE TIME_TO_EAT "
			"TIME_TO_SLEEP [REP]");
	prm->count = ft_atoi(argv[1]);
	if (prm->count <= 0)
		error("Invalid argument");
	prm->d_time = ft_atoi(argv[2]);
	if (prm->d_time <= 0)
		error("Invalid argument");
	prm->e_time = ft_atoi(argv[3]);
	if (prm->e_time <= 0)
		error("Invalid argument");
	prm->s_time = ft_atoi(argv[4]);
	if (prm->s_time <= 0)
		error("Invalid argument");
	prm->rep = -1;
	if (argc == 6)
		prm->rep = ft_atoi(argv[5]);
	if (prm->rep == 0)
		error("Invalid argument");
	prm->dead = false;
	prm->c_eat = 0;
	pthread_mutex_init(&prm->m_write, NULL);
	pthread_mutex_init(&prm->m_dead, NULL);
	pthread_mutex_init(&prm->m_eat, NULL);
}

void	init(t_philo **philo, t_prm *prm, int argc, char **argv)
{
	int				i;
	int				count;
	size_t			time;

	init_prm(prm, argc, argv);
	count = prm->count;
	time = timestamp();
	*philo = malloc(sizeof(t_philo) * count);
	if (!(*philo))
		return (quit(*philo));
	i = -1;
	while (++i < count)
	{
		(*philo)[i].m_rf = malloc(sizeof(pthread_mutex_t));
		if (!(*philo)[i].m_rf)
			return ;
		pthread_mutex_init((*philo)[i].m_rf, NULL);
		if (count > 1)
			(*philo)[(i + 1) % count].m_lf = (*philo)[i].m_rf;
		(*philo)[i].id = i + 1;
		(*philo)[i].prm = prm;
		(*philo)[i].last_eat = time;
	}
}
