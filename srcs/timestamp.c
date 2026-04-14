#include "../includes/philo.h"

size_t	get_time(void) {
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

size_t	simulation_elapsed_time(Simulation *sim) {
	pthread_mutex_lock(&sim->m_started);
	size_t start_time = sim->start_time;
	pthread_mutex_unlock(&sim->m_started);
	return get_time() - start_time;
}
