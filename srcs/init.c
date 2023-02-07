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
	int	i;

	i = -1;
	while (++i < philo->param->count)
		pthread_mutex_destroy(philo[i].m_rf);
	i = -1;
	while (++i < philo->param->count)
		free(philo[i].m_rf);
	free(philo);
}

void	init_param(t_param *param, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		error("Invalid arguments : ./philo N_PHILO TIME_TO_DIE TIME_TO_EAT "
			"TIME_TO_SLEEP [REP]");
	param->count = ft_atoi(argv[1]);
	if (param->count == 0)
		error("Invalid argument");
	param->d_time = ft_atoi(argv[2]);
	if (param->d_time == 0)
		error("Invalid argument");
	param->e_time = ft_atoi(argv[3]);
	if (param->e_time == 0)
		error("Invalid argument");
	param->s_time = ft_atoi(argv[4]);
	if (param->s_time == 0)
		error("Invalid argument");
	param->rep = -1;
	if (argc == 6)
		param->rep = ft_atoi(argv[5]);
	if (param->rep == 0)
		error("Invalid argument");
	param->dead = false;
	pthread_mutex_init(&param->m_write, NULL);
	pthread_mutex_init(&param->m_dead, NULL);
}

void	init(t_philo **philo, t_param *param)
{
	int	i;
	int	count;

	i = -1;
	count = param->count;
	*philo = malloc(sizeof(t_philo) * count);
	while (++i < count)
	{
		(*philo)[i].m_rf = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init((*philo)[i].m_rf, NULL);
		if (count > 1)
			(*philo)[(i + 1) % count].m_lf = (*philo)[i].m_rf;
		(*philo)[i].id = i;
		(*philo)[i].param = param;
		(*philo)[i].last_eat = timestamp();
	}
}
