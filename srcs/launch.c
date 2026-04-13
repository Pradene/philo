#include "../includes/philo.h"

void	pthread_quit(pthread_t *threads, size_t count) {
	while (count--) {
		if (pthread_join(threads[count], NULL)) {
			return ;
		}
	}
}

int	launch(Simulation *sim) {
	pthread_t	monitor;
	pthread_t	*threads;
	size_t		i;

	Philo *philos = sim->philos;
	threads = malloc(sizeof(pthread_t) * sim->philos_count);
	if (!threads) {
		return (1);
	}
	i = 0;
	pthread_mutex_lock(&sim->m_started);
	while (i < sim->philos_count) {
		if (pthread_create(&threads[i], NULL, &philo_routine, &philos[i])) {
			pthread_mutex_unlock(&sim->m_started);
			pthread_quit(threads, i);
			destroy_simulation(sim);
			free(threads);
			return (1);
		}
		++sim->philos_ready;
		++i;
	}
	pthread_mutex_unlock(&sim->m_started);
	sim->start_time = get_time();
	if (pthread_create(&monitor, NULL, &simulation_monitor, sim)) {
		pthread_quit(threads, i);
		destroy_simulation(sim);
		free(threads);
		return (1);
	}
    pthread_join(monitor, NULL);
	pthread_quit(threads, sim->philos_count);
	free(threads);
	return (0);
}
