#include "../includes/philo.h"

void	destroy_philos(Philo *philo, int count) {
	int	i;

	pthread_mutex_destroy(&philo->sim->m_write);
	pthread_mutex_destroy(&philo->sim->m_finished);
	pthread_mutex_destroy(&philo->sim->m_started);
	pthread_mutex_destroy(&philo->sim->m_dead);
	i = 0;
	while (i < count) {
		pthread_mutex_destroy(philo[i].right_fork);
		pthread_mutex_destroy(&philo[i].m_lasteat);
		free(philo[i].right_fork);
		++i;
	}
	free(philo);
}

void	destroy_simulation(Philo *philo) {
	int	i;

	pthread_mutex_destroy(&philo->sim->m_write);
	pthread_mutex_destroy(&philo->sim->m_finished);
	pthread_mutex_destroy(&philo->sim->m_started);
	pthread_mutex_destroy(&philo->sim->m_dead);
	i = 0;
	while (i < philo->sim->count) {
		pthread_mutex_destroy(philo[i].right_fork);
		pthread_mutex_destroy(&philo[i].m_lasteat);
		++i;
	}
	i = 0;
	while (i < philo->sim->count) {
		free(philo[i].right_fork);
		++i;
	}
	free(philo);
}
