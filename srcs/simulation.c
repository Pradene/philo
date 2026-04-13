#include "../includes/philo.h"

bool	is_philo_dead(Simulation *sim, Philo *p) {
	pthread_mutex_lock(&p->m_lasteat);
    size_t now = simulation_elapsed_time(sim);
	size_t elapsed_time_since_last_eat = now - p->last_eat;
	pthread_mutex_unlock(&p->m_lasteat);
	if (elapsed_time_since_last_eat >= sim->time_to_die) {
		pthread_mutex_lock(&sim->m_write);
		printf("%zu %d died\n", now, p->id);
		pthread_mutex_unlock(&sim->m_write);
		pthread_mutex_lock(&sim->m_dead);
		sim->dead = true;
		pthread_mutex_unlock(&sim->m_dead);
		return (true);
	}
	return (false);
}

void	*simulation_monitor(void *arg) {
	Simulation	*sim = (Simulation *)arg;
	Philo		*philos = sim->philos;
	size_t		i;

	while (true) {
		i = 0;
		while (i < sim->philos_count) {
			pthread_mutex_lock(&sim->m_finished);
			if (sim->philos_sated == sim->philos_count) {
				pthread_mutex_unlock(&sim->m_finished);
				return (NULL);
			}
			pthread_mutex_unlock(&sim->m_finished);
			if (is_philo_dead(sim, &philos[i])) {
				return (NULL);
			}
			++i;
		}
	}
}