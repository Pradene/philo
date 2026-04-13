#include "../includes/philo.h"

void	destroy_simulation(Simulation *sim) {
	pthread_mutex_destroy(&sim->m_write);
	pthread_mutex_destroy(&sim->m_finished);
	pthread_mutex_destroy(&sim->m_started);
	pthread_mutex_destroy(&sim->m_dead);
	for (size_t i = 0; i < sim->philos_count; ++i) {
		Philo philo = sim->philos[i];
		pthread_mutex_destroy(philo.right_fork);
		pthread_mutex_destroy(&philo.m_lasteat);
		free(philo.right_fork);
	}
	free(sim->philos);
}
