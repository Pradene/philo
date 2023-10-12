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

void	quit(t_prm *prm)
{
	pthread_mutex_destroy(&prm->m_write);
	pthread_mutex_destroy(&prm->m_finished);
	pthread_mutex_destroy(&prm->m_started);
	pthread_mutex_destroy(&prm->m_dead);
}

void	destroy_count(t_philo *philo, int count)
{
	int	i;

	pthread_mutex_destroy(&philo->prm->m_write);
	pthread_mutex_destroy(&philo->prm->m_finished);
	pthread_mutex_destroy(&philo->prm->m_started);
	pthread_mutex_destroy(&philo->prm->m_dead);
	i = -1;
	while (++i < count)
	{
		pthread_mutex_destroy(philo[i].m_rf);
		pthread_mutex_destroy(&philo[i].m_lasteat);
	}
	i = -1;
	while (++i < count)
		free(philo[i].m_rf);
	free(philo);
}

int	init_mutex(t_prm *prm)
{
	if (pthread_mutex_init(&prm->m_dead, NULL))
	{
		return (1);
	}
	if (pthread_mutex_init(&prm->m_started, NULL))
	{
		pthread_mutex_destroy(&prm->m_dead);
		return (1);
	}
	if (pthread_mutex_init(&prm->m_finished, NULL))
	{
		pthread_mutex_destroy(&prm->m_dead);
		pthread_mutex_destroy(&prm->m_started);
		return (1);
	}
	if (pthread_mutex_init(&prm->m_write, NULL))
	{
		pthread_mutex_destroy(&prm->m_dead);
		pthread_mutex_destroy(&prm->m_started);
		pthread_mutex_destroy(&prm->m_finished);
		return (1);
	}
	return (0);
}

int	init_prm(t_prm *prm, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (printf("Arguments : ./philo N_PHILO DIE_TIME "\
		"EAT_TIME SLEEP_TIME [REP]\n"), 1);
	prm->count = ft_atoi(argv[1]);
	if (prm->count <= 0 || prm->count > 200 || overflow(argv[1], prm->count))
		return (printf("Argument : N_PHILO must be between 1 and 200\n"), 1);
	prm->d_time = ft_atoi(argv[2]);
	if (prm->d_time <= 0 || overflow(argv[2], prm->d_time))
		return (printf("Argument : DIE_TIME must be an unsigned int\n"), 1);
	prm->e_time = ft_atoi(argv[3]);
	if (prm->e_time <= 0 || overflow(argv[3], prm->e_time))
		return (printf("Argument : EAT_TIME must be an unsigned int\n"), 1);
	prm->s_time = ft_atoi(argv[4]);
	if (prm->s_time <= 0 || overflow(argv[4], prm->s_time))
		return (printf("Argument : SLEEP_TIME must be an unsigned int\n"), 1);
	prm->rep = -1;
	if (argc == 6)
		prm->rep = ft_atoi(argv[5]);
	if (prm->rep == 0 || (argc == 6 && overflow(argv[5], prm->rep)))
		return (printf("Argument : REP must be an unsigned int\n"), 1);
	return (0);
}

void	init(t_philo **philo, t_prm *prm)
{
	int	i;

	*philo = malloc(sizeof(t_philo) * prm->count);
	if (!(*philo))
		return (quit(prm));
	i = -1;
	while (++i < prm->count)
	{
		(*philo)[i].m_rf = malloc(sizeof(pthread_mutex_t));
		if (!(*philo)[i].m_rf)
			break ;
		pthread_mutex_init((*philo)[i].m_rf, NULL);
		pthread_mutex_init(&(*philo)[i].m_lasteat, NULL);
		if (prm->count > 1)
			(*philo)[(i + 1) % prm->count].m_lf = (*philo)[i].m_rf;
		(*philo)[i].id = i + 1;
		(*philo)[i].prm = prm;
		(*philo)[i].eat = 0;
	}
	if (i < prm->count)
		destroy_count(*philo, i);
}
