/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:45:47 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/06 12:46:15 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	destroy_mutex(t_prm *prm)
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
		free(philo[i].m_rf);
	}
	free(philo);
}

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
