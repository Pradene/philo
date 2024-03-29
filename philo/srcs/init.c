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

int	init_philo(t_philo **p, t_prm *prm, int index)
{
	(*p)[index].m_rf = malloc(sizeof(pthread_mutex_t));
	if (!(*p)[index].m_rf)
		return (1);
	if (pthread_mutex_init((*p)[index].m_rf, NULL))
		return (free((*p)[index].m_rf), 1);
	if (pthread_mutex_init(&(*p)[index].m_lasteat, NULL))
	{
		pthread_mutex_destroy((*p)[index].m_rf);
		return (free((*p)[index].m_rf), 1);
	}
	if (prm->count > 1)
		(*p)[(index + 1) % prm->count].m_lf = (*p)[index].m_rf;
	(*p)[index].id = index + 1;
	(*p)[index].prm = prm;
	(*p)[index].eat = 0;
	(*p)[index].last_eat = timestamp();
	return (0);
}

int	init(t_philo **p, t_prm *prm)
{
	int	i;

	*p = malloc(sizeof(t_philo) * prm->count);
	if (!(*p))
		return (destroy_mutex(prm), 1);
	i = -1;
	while (++i < prm->count)
	{
		if (init_philo(p, prm, i))
		{
			destroy_count(*p, i);
			return (1);
		}
	}
	return (0);
}
